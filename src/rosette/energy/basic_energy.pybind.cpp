#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "rosette/energy/basic_energy.hpp"

namespace py = pybind11;
using namespace rosette::energy;
using namespace pybind11::literals;

// this fname must start with __PYBIND__ and be unique in the project
// actual python module path will be set by the directory this file is in
void __PYBIND__basic_energy(py::module& m) {
  m.def("basic_energy_safe", &basic_energy_safe<double>);
}