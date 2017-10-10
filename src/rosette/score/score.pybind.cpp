#include <pybind11/eigen.h>
#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>

#include "rosette/score/score.hpp"

namespace py = pybind11;
using namespace rosette::score;
using namespace pybind11::literals;

// this fname must start with __PYBIND__ and be unique in the project
// actual python module path will be set by the directory this file is in
void __PYBIND__rosette_score(py::module& m) {
  m.def("rosette_score_safe", &rosette_score_safe<double>);
}