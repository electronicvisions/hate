#!/usr/bin/env python
import os
from os.path import join
from waflib.extras.test_base import summary
from waflib.extras.symwaf2ic import get_toplevel_path

def depends(dep):
    # we can't build the documentation in the wafer app
    if (os.environ.get("SINGULARITY_APPNAME") is None) or ("wafer" not in os.environ.get("SINGULARITY_APPNAME")):
        dep('code-format')

def options(opt):
    opt.load('compiler_cxx')
    opt.load('gtest')
    # we can't build the documentation in the wafer app
    if (os.environ.get("SINGULARITY_APPNAME") is None) or ("wafer" not in os.environ.get("SINGULARITY_APPNAME")):
        opt.load("doxygen")

def configure(cfg):
    cfg.load('compiler_cxx')
    cfg.check_cxx(mandatory=True, header_name='cereal/cereal.hpp')
    cfg.load('gtest')
    # we can't build the documentation in the wafer app
    if (os.environ.get("SINGULARITY_APPNAME") is None) or ("wafer" not in os.environ.get("SINGULARITY_APPNAME")):
        cfg.load("doxygen")

def build(bld):
    bld.install_files(
        dest = '${PREFIX}/',
        files = bld.path.ant_glob('include/hate/**/*.(h)'),
        name = 'hate_header',
        relative_trick = True
    )

    # only export include path, header only lib.
    bld(
        target          = 'hate_inc',
        export_includes = 'include',
        use             = 'hate_header',
    )

    # unit tests
    bld(
        features = 'cxx cxxprogram gtest',
        source = bld.path.ant_glob('tests/**/*.cpp'),
        target='hate_tests',
        use = 'hate_inc',
    )

    # documentation
    # we can't build the documentation in the wafer app
    if ((os.environ.get("SINGULARITY_APPNAME") is None) or ("wafer" not in os.environ.get("SINGULARITY_APPNAME"))) and bld.env.DOXYGEN:
        bld(
            target = 'doxygen_hate',
            features = 'doxygen',
            doxyfile = bld.root.make_node(join(get_toplevel_path(), "code-format", "doxyfile")),
            doxy_inputs = 'include/hate',
            install_path = 'doc/hate',
            pars = {
                "PROJECT_NAME": "\"Helpers and Tools Ensemble library\"",
                "INCLUDE_PATH": join(get_toplevel_path(), "hate", "include"),
                "OUTPUT_DIRECTORY": join(get_toplevel_path(), "build", "hate", "doc")
            },
        )

    bld.add_post_fun(summary)
