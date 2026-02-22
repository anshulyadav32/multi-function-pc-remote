project('pcremote-linux', 'c',
  version: '1.0.0',
  default_options: ['warning_level=2', 'c_std=c11'],
)

gtk_dep = dependency('gtk4', version: '>= 4.10')
libsoup_dep = dependency('libsoup-3.0', version: '>= 3.0')
json_glib_dep = dependency('json-glib-1.0', version: '>= 1.6')

sources = [
  'src/main.c',
  'src/window.c',
  'src/connection.c',
  'src/media_view.c',
  'src/input_view.c',
  'src/file_view.c',
  'src/system_view.c',
  'src/screen_view.c',
]

executable('pcremote',
  sources,
  dependencies: [gtk_dep, libsoup_dep, json_glib_dep],
  install: true,
)

install_data('data/com.pcremote.app.desktop',
  install_dir: join_paths(get_option('datadir'), 'applications')
)

install_data('data/com.pcremote.app.gschema.xml',
  install_dir: join_paths(get_option('datadir'), 'glib-2.0', 'schemas')
)
