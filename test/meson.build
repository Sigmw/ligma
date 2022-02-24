catch_header = custom_target('catch-header',
    output: 'catch.hpp',
    command: [
        meson.current_source_dir() + '/update_catch.sh'
    ])

test_lisp = executable('test-lisp',
    sources: [
        catch_header,
        'test.cpp',
        'test_tokenize.cpp',
        'test_symbols.cpp',
        'test_builtin.cpp',
        'test_value.cpp',
        'test_parse.cpp',
        'test_eval.cpp',
        'test_env.cpp'
    ],
    include_directories: [
        test_inc,
        top_inc,
        main_inc
    ],
    link_with: [
        lisplib
    ]
)

test('Test lisp', test_lisp)
