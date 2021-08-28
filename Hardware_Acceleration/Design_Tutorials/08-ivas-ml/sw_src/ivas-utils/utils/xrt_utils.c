/*
* Copyright (C) 2020 - 2021 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software
* is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY
* KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
* EVENT SHALL XILINX BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
* OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE. Except as contained in this notice, the name of the Xilinx shall
* not be used in advertising or otherwise to promote the sale, use or other
* dealings in this Software without prior written authorization from Xilinx.
*/

/* Update of this file by the user is not encouraged */
#include <assert.h>
#include "xrt_utils.h"

#define ERROR_PRINT(...) {\
  do {\
    printf("[%s:%d] ERROR : ",__func__, __LINE__);\
    printf(__VA_ARGS__);\
    printf("\n");\
  } while(0);\
}

#define DEBUG_XRT_UTILS

#ifdef DEBUG_XRT_UTILS
#define DEBUG_PRINT(...) {\
  do {\
    printf("[%s:%d] ",__func__, __LINE__);\
    printf(__VA_ARGS__);\
    printf("\n");\
  } while(0);\
}
#else
#define DEBUG_PRINT(...) ((void)0)
#endif

struct sk_device_info
{
  int pid;
  void *device_handle;
  uint32_t dev_index;
};

#define MAX_DEVICES   (32)
#define P2ROUNDUP(x, align) (-(-(x) & -(align)))
#define WAIT_TIMEOUT 1000       // 1sec
#define ERT_START_CMD_PAYLOAD_SIZE ((1024 * sizeof(unsigned int)) - 2)  // ERT cmd can carry max 4088 bytes
#define MEM_BANK 0

struct sk_download_info
{
  int magic_flag;
  int32_t dev_count;
  struct sk_device_info dev_info[MAX_DEVICES];
};

int
alloc_xrt_buffer (xclDeviceHandle handle, unsigned int size,
    enum xclBOKind bo_kind, unsigned flags, xrt_buffer * buffer)
{
  if (handle == NULL || size == 0 || buffer == NULL) {
    ERROR_PRINT ("invalid arguments");
    return -1;
  }

  buffer->bo = xclAllocBO (handle, size, bo_kind, flags);
  if (buffer->bo == NULLBO) {
    ERROR_PRINT ("failed to allocate Device BO...");
    return -1;
  }

  buffer->user_ptr = xclMapBO (handle, buffer->bo, true);
  if (buffer->user_ptr == NULL) {
    ERROR_PRINT ("failed to map BO...");
    xclFreeBO (handle, buffer->bo);
    return -1;
  }

  if (bo_kind != XCL_BO_SHARED_VIRTUAL) {
    struct xclBOProperties p;
    if (xclGetBOProperties (handle, buffer->bo, &p)) {
      ERROR_PRINT ("failed to get physical address...");
      xclUnmapBO (handle, buffer->bo, buffer->user_ptr);
      buffer->user_ptr = NULL;
      xclFreeBO (handle, buffer->bo);
      return -1;
    }
    buffer->phy_addr = p.paddr;
  }
  buffer->size = size;
  return 0;
}

void
free_xrt_buffer (xclDeviceHandle handle, xrt_buffer * buffer)
{
  if (handle == NULL || buffer == NULL) {
    ERROR_PRINT ("invalid arguments");
    return;
  }
  if (buffer->user_ptr && buffer->size)
    xclUnmapBO (handle, buffer->bo, buffer->user_ptr);
  if (handle && buffer->bo > 0)
    xclFreeBO (handle, buffer->bo);
  memset (buffer, 0x00, sizeof (xrt_buffer));
}


static const struct axlf_section_header *
get_axlf_section2 (const struct axlf *top, enum axlf_section_kind kind)
{
  uint32_t i = 0;
  DEBUG_PRINT ("Finding section header for axlf section %d", kind);
  for (i = 0; i < top->m_header.m_numSections; i++) {
    DEBUG_PRINT ("Section is %d", top->m_sections[i].m_sectionKind);
    if (top->m_sections[i].m_sectionKind == kind) {
      DEBUG_PRINT ("Found section header for kind %d", kind);
      return &top->m_sections[i];
    }
  }
  ERROR_PRINT ("Section header for axlf section %d not found", kind);
  return NULL;
}

int
download_xclbin (const char *bit, unsigned deviceIndex, const char *halLog,
    xclDeviceHandle * handle, uuid_t * xclbinId)
{
  struct xclDeviceInfo2 deviceInfo;
  FILE *fptr = NULL;
  size_t size = 0;
  char *header = NULL;
  const xclBin *blob = NULL;
  const struct axlf *top = NULL;
  const struct axlf_section_header *ip = NULL;
  struct ip_layout *layout = NULL;
  int i;

  if (deviceIndex >= xclProbe ()) {
    ERROR_PRINT ("Device index not found");
    return -1;
  }

  *handle = xclOpen (deviceIndex, halLog, XCL_INFO);

  if (xclGetDeviceInfo2 (*handle, &deviceInfo)) {
    ERROR_PRINT ("Device information not found");
    goto error;
  }

  DEBUG_PRINT ("DSA = %s", deviceInfo.mName);
  DEBUG_PRINT ("Index = %d", deviceIndex);
  DEBUG_PRINT ("PCIe = GEN%ux%u", deviceInfo.mPCIeLinkSpeed,
      deviceInfo.mPCIeLinkWidth);
  DEBUG_PRINT ("OCL Frequency = %u", deviceInfo.mOCLFrequency[0]);
  DEBUG_PRINT ("DDR Bank = %u", deviceInfo.mDDRBankCount);
  DEBUG_PRINT ("Device Temp = %u", deviceInfo.mOnChipTemp);
  DEBUG_PRINT ("MIG Calibration = %u", deviceInfo.mMigCalib);

  if (!bit || !strlen (bit))
    return 0;

  fptr = fopen (bit, "rb");
  if (!fptr) {
    ERROR_PRINT ("fopen() with %s failed due to %s", bit, strerror (errno));
    goto error;
  }

  if (fseek (fptr, 0, SEEK_END) != 0) {
    ERROR_PRINT ("fseek() failed to SEEK_END with %s", strerror (errno));
    goto error;
  }

  size = ftell (fptr);
  if (size == (size_t) (-1)) {
    ERROR_PRINT ("ftell failed with %s", strerror (errno));
    goto error;
  }

  if (fseek (fptr, 0, SEEK_SET) != 0) {
    ERROR_PRINT ("fseek() failed to SEEK_SET with %s", strerror (errno));
    goto error;
  }

  header = (char *) calloc (1, size);
  if (header == NULL) {
    ERROR_PRINT ("failed to allocate memory");
    return -1;
  }

  if (fread (header, sizeof (char), size, fptr) != size) {
    ERROR_PRINT ("fread failed with %s", strerror (errno));
    goto error;
  }

  if (strncmp (header, "xclbin2", 8)) {
    ERROR_PRINT ("Invalid bitstream xclbin2 tag not present");
    goto error;
  }

  blob = (const xclBin *) header;
  if (xclLoadXclBin (*handle, blob)) {
    ERROR_PRINT ("Bitstream download failed");
    goto error;
  }
  DEBUG_PRINT ("Finished downloading bitstream %s ", bit);

  top = (const struct axlf *) header;
  ip = get_axlf_section2 (top, IP_LAYOUT);
  layout = (struct ip_layout *) (header + ip->m_sectionOffset);

  for (i = 0; i < layout->m_count; ++i) {
    if (layout->m_ip_data[i].m_type != IP_KERNEL)
      continue;
    DEBUG_PRINT ("index = %d, kernel name = %s, base_addr = %lx", i,
        layout->m_ip_data[i].m_name, layout->m_ip_data[i].m_base_address);
  }

  uuid_copy (*xclbinId, top->m_header.uuid);
  free (header);
  fclose (fptr);
  return 0;

error:
  if (header)
    free (header);
  if (fptr)
    fclose (fptr);
  xclClose (*handle);
  return -1;
}

static void
set_sk_dev_info (const void *dev_handle, const uint32_t index,
    struct sk_download_info *sk_info)
{
  if (sk_info == NULL || sk_info == MAP_FAILED)
    assert (0);

  sk_info->dev_info[sk_info->dev_count].device_handle = (void *) dev_handle;
  sk_info->dev_info[sk_info->dev_count].dev_index = index;
  sk_info->dev_info[sk_info->dev_count].pid = getpid ();
}

// TODO: instead of shared file, use flock
int
is_softkernel_downloaded (const void *dev_handle, const uint32_t index,
    const char *lock_file)
{
  const char *sk_download_file = lock_file;
  const int magic_mask = 0xFEED;
  int fd = -1;
  int idx;
  struct sk_download_info *sk_info = NULL;
  int ret;


  fd = open (sk_download_file, O_RDWR | O_CREAT | O_EXCL,
      S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd == -1 && errno == EEXIST) {

    fd = open (sk_download_file, O_RDWR, S_IRUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
      ERROR_PRINT ("failed to open file %s in RDWR mode", sk_download_file);
      goto exit;
    }

    sk_info =
        (struct sk_download_info *) mmap (NULL,
        sizeof (struct sk_download_info), PROT_WRITE | PROT_READ, MAP_SHARED,
        fd, 0);
    if (sk_info == MAP_FAILED) {
      ERROR_PRINT ("failed to map file %s in RDWR+Shared mode",
          sk_download_file);
      goto exit;
    }

    if (sk_info->magic_flag != magic_mask) {
      ERROR_PRINT ("wrong magic flag %x. expecting %x", sk_info->magic_flag,
          magic_mask);
      errno = EINVAL;
      goto exit;
    }

    for (idx = 0; idx < sk_info->dev_count && idx < MAX_DEVICES; idx++) {
      if (sk_info->dev_info[idx].device_handle == dev_handle
          || sk_info->dev_info[idx].dev_index == index) {
        DEBUG_PRINT
            ("Soft kernel download completed to device handle = %p device index = %d by Process Pid = %d, not downloading in this process",
            sk_info->dev_info[idx].device_handle,
            sk_info->dev_info[idx].dev_index, sk_info->dev_info[idx].pid);
        errno = EEXIST;
        goto exit;
      }
    }

    if (idx >= MAX_DEVICES || sk_info->dev_count >= MAX_DEVICES) {
      ERROR_PRINT ("exceeding max supported device count of %d", MAX_DEVICES);
      errno = EINVAL;
      goto exit;
    } else if (idx == sk_info->dev_count) {
      set_sk_dev_info (dev_handle, index, sk_info);
      DEBUG_PRINT ("Soft kernels downloaded to other devices");
      DEBUG_PRINT
          ("This is the first process pid = %d to attempt soft kernel download to device handle = %p dev_index = %d",
          sk_info->dev_info[sk_info->dev_count].pid,
          sk_info->dev_info[sk_info->dev_count].device_handle,
          sk_info->dev_info[sk_info->dev_count].dev_index);
      sk_info->dev_count++;
      munmap (sk_info, sizeof (struct sk_download_info));
      close (fd);
      return 0;
    }
    ERROR_PRINT ("Something went wrong ... bye :)");
    assert (0);
  } else if (fd == -1) {
    ERROR_PRINT ("unable to open file %s. reason : %s", sk_download_file,
        strerror (errno));
    goto exit;
  }

  ret = ftruncate (fd, sizeof (*sk_info));
  if (ret < 0) {
    ERROR_PRINT ("unable truncate fd. reason : %s", strerror (errno));
    goto exit;
  }

  sk_info =
      (struct sk_download_info *) mmap (NULL, sizeof *sk_info,
      PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
  if (sk_info == MAP_FAILED)
    goto exit;

  memset (sk_info, 0, sizeof (*sk_info));
  sk_info->magic_flag = magic_mask;
  set_sk_dev_info (dev_handle, index, sk_info);

  DEBUG_PRINT
      ("This is the first process pid = %d to attempt soft kernel download to device handle = %p dev_index = %d\n",
      sk_info->dev_info[sk_info->dev_count].pid,
      sk_info->dev_info[sk_info->dev_count].device_handle,
      sk_info->dev_info[sk_info->dev_count].dev_index);
  sk_info->dev_count++;

  munmap (sk_info, sizeof (struct sk_download_info));
  close (fd);
  return 0;

exit:
  if (sk_info)
    munmap (sk_info, sizeof (struct sk_download_info));
  if (fd != -1)
    close (fd);

  return errno;
}

static size_t
get_softkernel_size (const char *filename)
{
  struct stat st;

  if (!filename || stat (filename, &st) != 0) {
    ERROR_PRINT ("file %s is not valid. reason : %s", filename,
        strerror (errno));
    return -1;
  }
  DEBUG_PRINT ("%s file size is %ld", filename, (size_t) st.st_size);

  return (size_t) st.st_size;
}

static int
read_softkernel (const char *filepath, char *buf)
{
  size_t size;
  FILE *fptr;

  if (!filepath || !buf) {
    ERROR_PRINT ("invalid args : filepath %s and buf %p", filepath, buf);
    return -1;
  }

  fptr = fopen (filepath, "rb");
  if (!fptr) {
    ERROR_PRINT ("fopen() with %s failed due to %s", filepath,
        strerror (errno));
    return -1;
  }

  if (fseek (fptr, 0, SEEK_END) != 0) {
    ERROR_PRINT ("fseek() failed to SEEK_END with %s", strerror (errno));
    return -1;
  }

  size = ftell (fptr);
  if (size == (size_t) (-1)) {
    ERROR_PRINT ("ftell failed with %s", strerror (errno));
    return size;
  }

  if (fseek (fptr, 0, SEEK_SET) != 0) {
    ERROR_PRINT ("fseek() failed to SEEK_SET with %s", strerror (errno));
    return -1;
  }

  if (fread (buf, sizeof *buf, size, fptr) != size) {
    ERROR_PRINT ("fread failed with %s", strerror (errno));
    return -1;
  }

  (void) fclose (fptr);
  DEBUG_PRINT ("read soft kernel of size %ld", size);
  return 0;
}

int
configure_soft_kernel (xclDeviceHandle handle, const size_t sk_size,
    const char *sk_name, const uint64_t sk_paddr, const int start_idx,
    const int cu_count)
{
  unsigned exec_handle = NULLBO;
  void *exec_data = NULL;
  struct ert_configure_sk_cmd *ecmd = NULL;
  const int retry_max = 20;
  int attempts = 0;

  exec_handle =
      xclAllocBO (handle, DATA_SIZE, XCL_BO_SHARED_VIRTUAL, (1 << 31));
  if (exec_handle < 0) {
    ERROR_PRINT ("failed to allocate BO");
    goto error;
  }

  exec_data = xclMapBO (handle, exec_handle, true);
  if (exec_data == MAP_FAILED) {
    ERROR_PRINT ("failed to map exec_handle(0x%x) for device handle = 0x%p\n",
        exec_handle, handle);
    goto error;
  }


  ecmd = (struct ert_configure_sk_cmd *) exec_data;
  memset (ecmd, 0, DATA_SIZE);

  ecmd->state = ERT_CMD_STATE_NEW;
  ecmd->opcode = ERT_SK_CONFIG;
  ecmd->count = 11 + 2;
  ecmd->start_cuidx = start_idx;
  ecmd->num_cus = (cu_count <= 1 ? 1 : cu_count);
  ecmd->sk_size = sk_size;
  snprintf ((char *) ecmd->sk_name, 32, "%s", sk_name);
  ecmd->sk_addr = sk_paddr;

  DEBUG_PRINT
      ("cofiguring softkernel %s with start idx %d, cu_count %d and paddr %p",
      sk_name, start_idx, ecmd->num_cus, (void *) sk_paddr);


  if (xclExecBuf (handle, exec_handle)) {
    ERROR_PRINT ("unable to issue xclExecBuf");
    goto error;
  }


  while ((xclExecWait (handle, WAIT_TIMEOUT) == 0)
      || (ecmd->state != ERT_CMD_STATE_COMPLETED)) {
    if (attempts++ >= retry_max) {
      ERROR_PRINT
          ("unable to finish ERT_SK_CONFIG command after several retries ... bye");
      munmap (exec_data, DATA_SIZE);
      xclFreeBO (handle, exec_handle);
      goto error;
    }
  }
  munmap (exec_data, DATA_SIZE);
  xclFreeBO (handle, exec_handle);
  DEBUG_PRINT ("Successfully configured softkernel %s", sk_name);

  return 0;
error:
  if (exec_data)
    munmap (exec_data, DATA_SIZE);
  if (exec_handle != NULLBO)
    xclFreeBO (handle, exec_handle);
  return -1;
}

int
download_softkernel (const char *filename, const char *sk_name,
    xclDeviceHandle handle, const int start_idx, const int cu_count)
{
  size_t fsize = get_softkernel_size (filename);
  struct xclBOProperties sk_bo_props;
  unsigned int sk_bhandle = NULLBO;
  char *sk_vaddr = NULL;
  uint64_t sk_paddr = -1;
  int ret;

  printf ("Going to download softkernel...\n");

  if (fsize <= 0) {
    ERROR_PRINT ("unable to get soft kernel size");
    goto error;
  }
  fsize = P2ROUNDUP (fsize, getpagesize ());


  sk_bhandle = xclAllocBO (handle, fsize, XCL_BO_DEVICE_RAM, MEM_BANK);
  if (sk_bhandle < 0) {
    ERROR_PRINT ("unable to allocate BO to download Soft Kernel");
    goto error;
  }

  sk_vaddr = (char *) xclMapBO (handle, sk_bhandle, true);
  if (sk_vaddr == (char *) MAP_FAILED) {
    ERROR_PRINT ("unable to map BO");
    goto error;
  }

  sk_paddr =
      !xclGetBOProperties (handle, sk_bhandle,
      &sk_bo_props) ? sk_bo_props.paddr : -1;
  if (sk_paddr == (uint64_t) (-1)) {
    ERROR_PRINT ("failed to get BO properties");
    goto error;
  }


  if (read_softkernel (filename, sk_vaddr)) {
    goto error;
  }

  if (xclSyncBO (handle, sk_bhandle, XCL_BO_SYNC_BO_TO_DEVICE, fsize, 0)) {
    ERROR_PRINT ("unable to sync to softkernel to device");
    goto error;
  }

  ret =
      configure_soft_kernel (handle, fsize, sk_name, sk_paddr, start_idx,
      cu_count);
  if (ret < 0) {
    ERROR_PRINT ("failed to configure softkernel");
    goto error;
  }
#if 0                           // TODO: Free this memory at end or check which is the better place
  if (sk_vaddr)
    munmap (sk_vaddr, fsize);
  if (sk_bhandle != NULLBO)
    xclFreeBO (handle, sk_bhandle);
#endif
  return 0;

error:
  if (sk_vaddr)
    munmap (sk_vaddr, fsize);
  if (sk_bhandle != NULLBO)
    xclFreeBO (handle, sk_bhandle);
  return -1;
}

int
send_softkernel_command (xclDeviceHandle handle, xrt_buffer * sk_ert_buf,
    unsigned int *payload, unsigned int num_idx, unsigned int cu_mask,
    int timeout)
{
  struct ert_start_kernel_cmd *ert_cmd =
      (struct ert_start_kernel_cmd *) (sk_ert_buf->user_ptr);
  int ret = 0;
  int retry_cnt = 0;

  if (NULL == sk_ert_buf || NULL == payload
      || (num_idx * sizeof (unsigned int)) > ERT_START_CMD_PAYLOAD_SIZE
      || !num_idx) {
    ERROR_PRINT
        ("invalid arguments. sk_buf = %p, payload = %p and num idx = %d",
        sk_ert_buf, payload, num_idx);
    return -1;
  }

  ert_cmd->state = ERT_CMD_STATE_NEW;
  ert_cmd->opcode = ERT_SK_START;


  ert_cmd->extra_cu_masks = 3;

  if (cu_mask > 31) {
    ert_cmd->cu_mask = 0;
    if (cu_mask > 63) {
      ert_cmd->data[0] = 0;
      if (cu_mask > 96) {
        ert_cmd->data[1] = 0;
        ert_cmd->data[2] = (1 << (cu_mask - 96));
      } else {
        ert_cmd->data[1] = (1 << (cu_mask - 64));
        ert_cmd->data[2] = 0;
      }
    } else {
      ert_cmd->data[0] = (1 << (cu_mask - 32));
    }
  } else {
    ert_cmd->cu_mask = (1 << cu_mask);
    ert_cmd->data[0] = 0;
    ert_cmd->data[1] = 0;
    ert_cmd->data[2] = 0;
  }


  ert_cmd->count = num_idx + 4;
  memcpy (&ert_cmd->data[3], payload, num_idx * sizeof (unsigned int));

  ret = xclExecBuf (handle, sk_ert_buf->bo);
  if (ret < 0) {
    ERROR_PRINT ("[handle %p & bo %d] ExecBuf failed with ret = %d. reason : %s", handle, sk_ert_buf->bo, ret, strerror (errno));
    return ret;
  }

  do {
    ret = xclExecWait (handle, timeout);
    if (ret < 0) {
      ERROR_PRINT ("ExecWait ret = %d. reason : %s", ret, strerror (errno));
      return ret;
    } else if (!ret) {
      if (retry_cnt++ >= 10) {
        ERROR_PRINT ("[handle %p] ExecWait ret = %d. reason : %s", handle, ret, strerror (errno));
	return -1;
      }
      printf ("[handle %p & bo %d] timeout...retry execwait\n", handle, sk_ert_buf->bo);
    }
  } while (ert_cmd->state != ERT_CMD_STATE_COMPLETED);

  return 0;
}
