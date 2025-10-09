#include <TickSimulators.h>
#include <pybind11/pybind11.h>

namespace py = pybind11;

PYBIND11_MODULE(random_tick_simulator, m) {
    py::class_<RandomTickSimulator>(m, "RandomTickSimulator")
        .def(py::init<double, double, double, uint64_t, uint64_t>())
        .def("start", &RandomTickSimulator::start)
        .def("stop", &RandomTickSimulator::stop)
        .def("getValue", &RandomTickSimulator::getValue)
        .def("getTimestamp", &RandomTickSimulator::getTimestamp);
}
