project('ligma', 'cpp')

add_global_arguments('-std=c++11', language : 'cpp')
add_global_arguments('-fPIC', language : 'cpp')
add_global_arguments('-Wall', language : 'cpp')
add_global_arguments('-Werror', language : 'cpp')
add_global_arguments('-Wuninitialized', language : 'cpp')

main_inc = include_directories('include')
top_inc = include_directories('.')
test_inc = include_directories('test')

cpp_compiler = meson.get_compiler('cpp')
readline = cpp_compiler.find_library('readline', required: false)
if readline.found()
  add_project_arguments('-DHAVE_READLINE', language: 'cpp')
endif

subdir('src')
subdir('test')
