
reid_dep = cc.find_library('vitis_ai_library-reid')

ivas_xreid = library('ivas_xreid',
  'src/ivas_xreid.cpp',
  cpp_args : [gst_plugins_ivas_args, '-std=c++11'],
  include_directories : [configinc],
  dependencies : [gstvideo_dep, gst_dep, xrt_dep, jansson_dep, ivasutils_dep, ivasmeta_dep, opencv_dep, reid_dep],
  install : true,
  install_dir : ivas_videolib_install_path,
)

pkgconfig.generate(ivas_xreid, install_dir : plugins_pkgconfig_install_dir)
