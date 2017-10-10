#!/usr/bin/env python

import sys
import os
import traceback
sys.path.append(os.path.dirname(__file__))  # not sure why sometimes necessary
from build_utils import build_and_test, build_and_run_gtest_auto, _VERBOSE, in_CI_environment


def run_tests(projname):
    try:
        build_and_run_gtest_auto(projname)
        gtest_ran = True
    except NotImplementedError:
        gtest_ran = False
    # print("gtest_ran:", gtest_ran)
    if not gtest_ran:
        print('gtest no run, build_and_test')
        return build_and_test(projname)


def isdocfile(fname):
    return fname.endswith('/docs/conf.py') or fname.endswith('.rst')


def run_docs():
    rstfiles = [arg for arg in sys.argv[1:] if isdocfile(arg)]
    assert rstfiles
    rstfile = rstfiles[0]
    docsloc = rstfile[:rstfile.rfind('/docs/') + 5]
    print(docsloc)
    shell_cmd = "cd %s && make html 2>&1 | tee ../log/subl_build.log" % docsloc
    print(shell_cmd)
    assert not os.system(shell_cmd)
    url = docsloc + "/_build/html"
    os.system('google-chrome %s' % url)


def run(projname):
    if any(isdocfile(arg) for arg in sys.argv[1:]):
        print('run_docs')
        sys.stdout.flush()
        return run_docs(projname)
    else:
        print('run_tests')
        sys.stdout.flush()
        return run_tests(projname)


if __name__ == '__main__':
    projname = 'rosette'
    if _VERBOSE:
        print('== build_and_test.py start ==')
        if in_CI_environment():
            print('== in ci ==')
        sys.stdout.flush()
    try:
        sys.exit(run(projname))
    except Exception as e:
        t, v, tb = sys.exc_info()
        if _VERBOSE:
            with open('.ERROR', 'w') as out:
                out.write(str(e))
        print('==========================================================')
        print("error running build_and_test, traceback:")
        print('==========================================================')
        print('Exception:', e)
        print('Exception Type:', type(e))
        print('Traceback:')
        sys.stdout.flush()
        sys.stderr.flush()
        traceback.print_tb(tb)
