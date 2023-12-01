#ifndef RVL_H
#define RVL_H

#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>

namespace py = pybind11;

void HelloWorld();

py::array_t<int> cpArray(py::array_t<int> fromArr, uint len);

float somethingEasier(py::array_t<float> arr);

py::array_t<short> DecompressRVL(py::array_t<char> inputPy, int numPixels);

PYBIND11_MODULE(rvl, mod) {
    mod.def("say_hello", &HelloWorld, "Hello World.");
    mod.def("cp_array", &cpArray, "copies one array to the other");
    mod.def("something_easier", &somethingEasier, "prints and returns the first element of a numpy float array");
    mod.def("decompress_rvl", &DecompressRVL, "returns a numpy array containing decompressed depths");
}

#endif // RVL_H