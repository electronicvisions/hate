#!/usr/bin/env python

def options(opt):
    pass

def configure(cfg):
    pass

def build(bld):
    # only export include path, header only lib.
    bld(
        target          = 'hate_inc',
        export_includes = 'include',
    )
