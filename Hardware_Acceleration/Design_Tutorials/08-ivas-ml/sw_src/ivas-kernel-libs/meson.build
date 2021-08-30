project('ivas-video-libs', 'c', 'cpp',
  version : '1.0.0',
  meson_version : '>= 0.48.0',
  default_options : [ 'warning_level=1',
                      'buildtype=debugoptimized' ])

gst_version = '1.16.2'
version_arr = gst_version.split('.')
gst_version_major = version_arr[0].to_int()
gst_version_minor = version_arr[1].to_int()
gst_version_micro = version_arr[2].to_int()
 if version_arr.length() == 4
  gst_version_nano = version_arr[3].to_int()
else
  gst_version_nano = 0
endif
gst_version_is_dev = gst_version_minor % 2 == 1 and gst_version_micro < 90

glib_req = '>= 2.44.0'
gst_req = '>= @0@.@1@.0'.format(gst_version_major, gst_version_minor)
xrt_req = '>= 2.6' #XRT 2020.1

glib_deps = [dependency('glib-2.0', version : glib_req, fallback: ['glib', 'libglib_dep']),
             dependency('gobject-2.0', fallback: ['glib', 'libgobject_dep'])]

gst_plugins_ivas_args = ['-DHAVE_CONFIG_H']

configinc = include_directories('.')

api_version = '1.0'
soversion = 0
# maintaining compatibility with the previous libtool versioning
# current = minor * 100 + micro
curversion = gst_version_minor * 100 + gst_version_micro
libversion = '@0@.@1@.0'.format(soversion, curversion)

plugins_install_dir = join_paths(get_option('libdir'), 'gstreamer-1.0')
plugins = []

cdata = configuration_data()
cc = meson.get_compiler('c')

cdata.set_quoted('LIBDIR', join_paths(get_option('prefix'), get_option('libdir')))
cdata.set_quoted('LOCALEDIR', join_paths(get_option('prefix'), get_option('localedir')))

# supporting 64-bit x86 family & ARM 64-bit compilers only

if host_machine.cpu_family() == 'x86_64'
  message ('compiling for PCIe platform')
  cdata.set('XLNX_PCIe_PLATFORM', 1)
elif host_machine.cpu_family() == 'aarch64'
  message ('compiling for Embedded platform')
  cdata.set('XLNX_EMBEDDED_PLATFORM ', 1)
else
  error('not supporting host machine families other than x86_64 & aarch64')
endif

warning_flags = [
  '-Wmissing-declarations',
  '-Wredundant-decls',
  '-Wwrite-strings',
  '-Winit-self',
  '-Wmissing-include-dirs',
  '-Wno-multichar',
  '-Wvla',
  '-Wpointer-arith',
]

warning_c_flags = [
  '-Wmissing-prototypes',
  '-Wdeclaration-after-statement',
  '-Wold-style-definition',
]

foreach extra_arg : warning_flags
  if cc.has_argument (extra_arg)
    add_project_arguments([extra_arg], language: 'c')
  endif
endforeach

foreach extra_arg : warning_c_flags
  if cc.has_argument (extra_arg)
    add_project_arguments([extra_arg], language: 'c')
  endif
endforeach

add_project_arguments(['-I' + meson.current_source_dir() + '/../install/usr/include/'], language: ['c', 'cpp'])
add_project_arguments(['-I' + meson.current_source_dir() + '/../install/usr/include/gstreamer-1.0'], language: ['c', 'cpp'])

check_headers = [
  ['HAVE_STDIO_H', 'stdio.h'],
  ['HAVE_DLFCN_H', 'dlfcn.h'],
]

foreach h : check_headers
  if cc.has_header(h.get(1))
    cdata.set(h.get(0), 1)
  endif
endforeach

# Mandatory GST deps
gst_dep = dependency('gstreamer-1.0', version : gst_req,
  fallback : ['gstreamer', 'gst_dep'])
gstbase_dep = dependency('gstreamer-base-1.0', version : gst_req,
  fallback : ['gstreamer', 'gst_base_dep'])
gstvideo_dep = dependency('gstreamer-video-1.0', version : gst_req,
    fallback : ['gst-plugins-base', 'video_dep'])

# XRT dependency
xrt_dep = dependency('xrt', version : xrt_req, required : true)

#ivasutils dependency
ivasutils_dep = cc.find_library('ivasutil', dirs : ['/usr/lib/','/usr/lib/', meson.current_source_dir() + '/../install/usr/lib/'])

#gstivasmeta dependency
ivasmeta_dep = cc.find_library('gstivasmeta-' + api_version, dirs : ['/usr/lib/', meson.current_source_dir() + '/../install/usr/lib/'])


# External dependency
jansson_dep = dependency('jansson', version : '>= 2.8', required: true)

opencv_dep = dependency('opencv', version : '>=3.4.0', required: true)

glog_dep = cc.find_library('glog', dirs : ['/usr/lib/x86_64-linux-gnu/'])

ivas_videolib_install_path = ['/usr/lib/ivas/']

plugins_pkgconfig_install_dir = join_paths(plugins_install_dir, 'pkgconfig')
if get_option('default_library') == 'shared'
  # If we don't build static plugins there is no need to generate pc files
  plugins_pkgconfig_install_dir = disabler()
endif

python3 = import('python').find_installation()
pkgconfig = import('pkgconfig')

# common kernel libraries for PCIe & embedded platforms
foreach kernel : ['ivas_xdpuinfer', 'ivas_xcrop', 'ivas_xresize_bgr', 'ivas_xreid', 'ivas_xswbbox']
  if not get_option(kernel).disabled()
    subdir(kernel)
  endif
endforeach

configure_file(output : 'config.h', configuration : cdata)

run_command(python3, '-c', 'import shutil; shutil.copy("hooks/pre-commit.hook", ".git/hooks/pre-commit")')
