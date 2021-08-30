
ivas_xresize_bgr = library('ivas_xresize_bgr',
  'src/ivas_xresize_bgr.c',
#  cpp_args : [gst_plugins_ivas_args, '-std=c++11'],
  c_args : [gst_plugins_ivas_args],
  include_directories : [configinc],
  dependencies : [jansson_dep, ivasutils_dep],
  install : true,
  install_dir : ivas_videolib_install_path,
)

pkgconfig.generate(ivas_xresize_bgr, install_dir : plugins_pkgconfig_install_dir)
