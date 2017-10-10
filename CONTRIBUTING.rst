Contributing
=================

.. inclusion-marker-do-not-remove


How to contribute
------------------

Install Dependencies
~~~~~~~~~~~~~~~~~~~~~~

you need python3, and a c++11 capable compiler. maybe boost will be a requirement at some point. if you have those things, you can install all the other dependencies like so:

- python3 -m ensurepip --upgrade
- python3 -m pip install -rrequirements (--user for non-root, --upgrade to upgrade)

Building
~~~~~~~~~
The build system is driven with cmake. There is a wrapper in setup.py which calls cmake with the right options to put stuff in the right places. the CMakeLists.txt file calls a couple of scripts in the tools directory (which should probably get cleaned up and put in a separate package) that coordinate c++/python integration.

Ways to build:

- python3 tools/build_and_test.py --inplace
- python3 tools/build_and_test.py --inplace path/to/my_module[.py or _test.py] for one test
- python3 setup.py sdist bdist_wheel
- pip install . (could use --develop or whatever)
- mkdir build; cd build; cmake ..; make


Testing
~~~~~~~~
Pytest is the main test harness. all gtests in *.gtest.cpp files are build into a binary gtest_all, which should integrate with pytest using pytest-cpp. Encourage use of hypothesis_. See test_basic_energy_safe_ for a cool example randomly generating test cases and validating based on a test oracle strategy.

Ways to run tests:

- python3 tools/build_and_test.py --inplace
- python3 tools/build_and_test.py --inplace <file> will run one test with "smart" dispatching
- pip install .; python3 -mpytest --pyargs rosette

Adding a C++ module
~~~~~~~~~~~~~~~~~~~~~~

We use pybind11_, a great library for python/c++ interoperability. To add a c++ module, create a file called path/to/my_module.pybind.cpp which defines a function:

.. code-block:: C++


    #include <pybind11/pybind11.h>
    namespace py = pybind11;
    void __PYBIND__something_unique_like_path(py::module &m) {
        // pybind11 code here, adding to py::module m
        // will automagically end up in python module path.to.my_module
    }



This function will be auto-detected by the build system and called with a pybind11_s module rif.path.to.my_module, i.e. module namespaces based on the file path. If your filename starts in caps: path/to/MyClass.pybind.cpp, the module m will instead be rif.path.to. That is, build system will assume this is a class that you will bind and should not be a module namespace. (This inconsistency is maybe not a good idea?)

Adding a gtest for c++ module
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Create a file called path/to/my_module.gtest.cpp and all the tests it contains will be run automatically. you can run only the gtests by either 'find build* -name gtest_all' and running the appropriate one, or 'python3 tools/build_and_test.py path/to/my_module.[hpp or .gtest.cpp]'. this is handy for editor integration.


.. code-block:: C++

    #include <gtest/gtest.h>
    namespace rosette {
    namespace energy {
    namespace my_gtest {
        TEST(test_group, test_name) {
            ASSERT_EQ(1, 1);
    }}}



Editor Integration
~~~~~~~~~~~~~~~~~~~~

To run c++ tests, pytests, build docs, or test everything based on context, use: "python3 tools/build_and_test.py --inplace $file" from your editor. Will knows how to do it in sublime text.

Releases
~~~~~~~~~~
want to set it up so any push to 'release' branch goes up to pypi automatically. annoying, because linux releasese *must* use this manylinux1 platform, which is a least-common-denominator of all supported platforms. CentOS 5... yuck! A docker image is available. The following is a work in progress.

.. code-block:: bash

    sudo docker run -i -t -v `pwd`:/io quay.io/pypa/manylinux1_x86_64 /bin/bash

.. code-block:: bash

    wget https://www.dropbox.com/s/15yqynnjj10k09c/cmake39centos5.tar.bz2?dl=0
    bzip2 -d cmake39centos5.tar.bz2
    tar -xf cmake39centos5.tar
    export PATH=/opt/python/cp36-cp36m/bin:/opt/rh/devtoolset-2/root/usr/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin

    git clone https://github.com/willsheffler/rosette.git
    cd rosette
    /opt/python/cp36-cp36m/bin/python -m pip install -r requirements.txt
    python setup.py sdist bdist_wheel




Contributors
-------------
- Frank DiMaio
- Will Sheffler


.. _pybind11: http://pybind11.readthedocs.io/en/master

.. _hypothesis: http://hypothesis.works

.. _test_basic_energy_safe: _modules/rosette/energy/basic_energy_test.html#test_basic_energy_safe
