ivas_xm2m = library('gstivas_xm2m', 'gstivas_xm2m.cpp',
  cpp_args : [gst_plugins_ivas_args, '-std=c++11'],
  include_directories : [configinc, libsinc],
  dependencies : [gstvideo_dep, gst_dep, xrt_dep, dl_dep, jansson_dep, gstallocators_dep, gstivasalloc_dep, uuid_dep, ivasutils_dep, xrtutil_dep],
  install : true,
  install_dir : plugins_install_dir,
)

pkgconfig.generate(ivas_xm2m, install_dir : plugins_pkgconfig_install_dir)
plugins += [ivas_xm2m]
