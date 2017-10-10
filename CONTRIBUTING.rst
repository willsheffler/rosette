Contributing
=================

.. inclusion-marker-do-not-remove


How to contribute
------------------

Install Dependencies
~~~~~~~~~~~~~~~~~~~~~~

you need python3, and a c++11 capable compiler and maybe some other stuff

- python3 -m ensurepip --upgrade
- python3 -m pip install -rrequirements (--user for non-root, --upgrade to upgrade)

Building
~~~~~~~~~
Options:

- python3 tools/build_and_test.py --inplace
- python3 tools/build_and_test.py --inplace path/to/my_module[.py or _test.py] for one test
- python3 setup.py sdist bdist_wheel
- pip install . (could use --develop or whatever)
- mkdir build; cd build; cmake ..; make

Testing
~~~~~~~~
Pytest is the main test harness. all gtests in *.gtest.cpp files are build into a gtest_all, which is supposed to get run. Encourage use of hypothesis http://hypothesis.works. Options to run test

- python3 tools/build_and_test.py --inplace
- pip install .; python3 -mpytest --pyargs rosette

Adding a c++ module
~~~~~~~~~~~~~~~~~~~~~~

Create a file called path/to/my_module.pybind.cpp which defines a function

.. code-block:: C++


    #include <pybind11/pybind11.h>
    namespace py = pybind11;
    void __PYBIND__something_unique_like_path(py::module &m) {
        // pybind11 code here, adding to py::module m
        // will automagically end up in python module path.to.my_module
    }

http://pybind11.readthedocs.io/en/master/

This function will be auto-detected by the build system and called with a pybind module rif.path.to.my_module, i.e. module namespaces based on the file path. If your filename starts in caps: path/to/MyClass.pybind.cpp, the module m will instead be rif.path.to. That is, build system will assume this is a class that you will bind and should not be a module namespace. (This inconsistency is maybe not a good idea?)

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

Contributors
-------------
- Frank DiMaio
- Will Sheffler

