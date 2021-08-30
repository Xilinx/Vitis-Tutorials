gstivas_xfilter = library('gstivas_xfilter', 'gstivas_xfilter.c',
  c_args : gst_plugins_ivas_args,
  include_directories : [configinc, libsinc],
  dependencies : [gstvideo_dep, gst_dep, gstivasmeta_dep, gstivasalloc_dep, xrt_dep, dl_dep, jansson_dep, gstallocators_dep, uuid_dep, ivasutils_dep, xrtutil_dep],
  install : true,
  install_dir : plugins_install_dir,
)

pkgconfig.generate(gstivas_xfilter, install_dir : plugins_pkgconfig_install_dir)
plugins += [gstivas_xfilter]
