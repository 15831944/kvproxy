cc_library(
    name = 'kvproxy',
    srcs = [
        'file-kvproxy.cpp',
        'kvproxy-adapter.cpp',
        'kvproxy-manager.cpp',
        'kvproxy-type.cpp',
        'kvproxy-type-register.cpp',
        'normal-error.cpp',
        'kvproxy-status-decorator.cpp',
        'POD-mmap-file.cpp'
    ],
    deps = [
        '//thirdparty/boost:boost',
        '//thirdparty/glog:glog',
    ],
    warning = []
)

cc_test(
    name = 'kvproxy_test',
    srcs = 'kvproxy_test.cpp',
    deps = [
        ':kvproxy',
    ],
    testdata = 'kvproxy.conf'
)

cc_test(
    name = 'mock-kvproxy',
    srcs = 'mock-kvproxy_test.cpp',
    deps = [
        ':kvproxy',
    ],
    testdata = 'kvproxy.conf'
)
