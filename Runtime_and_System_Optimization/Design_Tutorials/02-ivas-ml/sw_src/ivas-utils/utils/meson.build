xrtutil_sources = ['xrt_utils.c']

xrtutils = library('xrtutil',
  xrtutil_sources,
  c_args : gst_plugins_ivas_args,
  include_directories : [configinc],
  install : true,
  dependencies : [xrt_dep, uuid_dep],
)

xrtutils_dep = declare_dependency(link_with : xrtutils, 
  include_directories : [utilsinc],
  dependencies : [xrt_dep])

ivasutil_sources = ['ivas_kernel_utils.c']

ivasutil = library('ivasutil',
  ivasutil_sources,
  c_args : gst_plugins_ivas_args,
  include_directories : [configinc],
  install : true,
  dependencies : [xrt_dep],
)

#IVAS Common Headers to install
#ivas_common_headers = ['ivas/ivas_kernel.h', 'ivas/ivasmeta.h']
ivas_common_headers = ['ivas/ivas_kernel.h', 'ivas/ivasmeta.h', 'xrt_utils.h', 'ivas/ivaslogs.h']
install_headers(ivas_common_headers, subdir : 'ivas/')

ivasutils_dep = declare_dependency(link_with : ivasutil,
  include_directories : [utilsinc],
  dependencies : [xrt_dep])
