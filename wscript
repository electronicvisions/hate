#!/usr/bin/env python
from waflib.extras.test_base import summary

def options(opt):
    opt.load('compiler_cxx')
    opt.load('gtest')

def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.check_cxx(mandatory=True, header_name='cereal/cereal.hpp')
    cfg.load('gtest')

def build(bld):
    # only export include path, header only lib.
    bld(
        target          = 'hate_inc',
        export_includes = 'include',
    )

    # unit tests
    bld(
        features = 'cxx cxxprogram gtest',
        source = bld.path.ant_glob('tests/**/*.cpp'),
        target='hate_tests',
        use = 'hate_inc',
    )

    bld.add_post_fun(summary)
