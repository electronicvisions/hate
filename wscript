#!/usr/bin/env python

def options(opt):
    opt.load('compiler_cxx')

def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.check_cxx(mandatory=True, header_name='cereal/cereal.hpp')

def build(bld):
    # only export include path, header only lib.
    bld(
        target          = 'hate_inc',
        export_includes = 'include',
    )
