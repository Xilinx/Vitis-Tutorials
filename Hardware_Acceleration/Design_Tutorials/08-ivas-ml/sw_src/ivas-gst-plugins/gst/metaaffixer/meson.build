gstivas_xmetaaffixer = library('gstivas_xmetaaffixer', 'gstivas_xmetaaffixer.c',
  c_args : gst_plugins_ivas_args,
  include_directories : [configinc, libsinc],
  dependencies : [gstvideo_dep, gst_dep, gstivasmeta_dep],
  install : true,
  install_dir : plugins_install_dir,
)

message ('compiling metaaffixer')
pkgconfig.generate(gstivas_xmetaaffixer, install_dir : plugins_pkgconfig_install_dir)
plugins += [gstivas_xmetaaffixer]
