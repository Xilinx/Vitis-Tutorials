# IVAS metadata
meta_sources = ['gstivasmeta.c']

gstivasmeta = library('gstivasmeta-' + api_version,
  meta_sources,
  c_args : gst_plugins_ivas_args,
  include_directories : [configinc],
  version : libversion,
  soversion : soversion,
  install : true,
  dependencies : [gst_dep, gstbase_dep, gstvideo_dep],
)
gstivasmeta_dep = declare_dependency(link_with : [gstivasmeta], dependencies : [gst_dep, gstbase_dep])

#IVAS allocator using XRT
alloc_sources = ['gstivasallocator.c']

gstivasalloc = library('gstivasallocator-' + api_version,
  alloc_sources,
  c_args : gst_plugins_ivas_args,
  include_directories : [configinc, libsinc],
  version : libversion,
  soversion : soversion,
  install : true,
  dependencies : [gst_dep, gstbase_dep, gstvideo_dep, xrt_dep, gstallocators_dep],
)
gstivasalloc_dep = declare_dependency(link_with : [gstivasalloc], dependencies : [gst_dep, gstbase_dep])

#IVAS bufferpool with stride and elevation
ivaspool_sources = ['gstivasbufferpool.c']

gstivaspool = library('gstivasbufferpool-' + api_version,
  ivaspool_sources,
  c_args : gst_plugins_ivas_args,
  include_directories : [configinc, libsinc],
  version : libversion,
  soversion : soversion,
  install : true,
  dependencies : [gst_dep, gstbase_dep, gstvideo_dep, xrt_dep, gstivasalloc_dep],
)

# IVAS Input Inference metadata
inpinfermeta_sources = ['gstivasinpinfer.c']

gstivasinpinfermeta = library('gstivasinpinfermeta-' + api_version,
  inpinfermeta_sources,
  c_args : gst_plugins_ivas_args,
  include_directories : [configinc],
  version : libversion,
  soversion : soversion,
  install : true,
  dependencies : [gst_dep, gstbase_dep, gstvideo_dep],
)
gstivasinpinfermeta_dep = declare_dependency(link_with : [gstivasinpinfermeta], dependencies : [gst_dep, gstbase_dep])

#IVAS GST Headers to install
ivas_gst_headers = ['gstivasmeta.h',
                    'gstivasallocator.h',
                    'gstivasbufferpool.h',
                    'gstivasinpinfer.h']

install_headers(ivas_gst_headers, subdir : 'gstreamer-1.0/gst/ivas/')

gstivaspool_dep = declare_dependency(link_with : [gstivaspool], dependencies : [gst_dep, gstbase_dep])
