
ivas_xswbbox = library('ivas_xswbbox',
  'src/ivas_xswbbox.cpp',
#  cpp_args : [gst_plugins_ivas_args, '-std=c++11'],
  c_args : [gst_plugins_ivas_args],
  include_directories : [configinc],
  dependencies : [jansson_dep, ivasutils_dep, gst_dep, ivasmeta_dep],
  install : true,
  install_dir : ivas_videolib_install_path,
)

pkgconfig.generate(ivas_xswbbox, install_dir : plugins_pkgconfig_install_dir)
