#include "ownrecord.h"
#include "liburing.h"

void FileWriter::write_one(py::bytearray data)
{

}



int add(int i, int j){
    return i+j;
}

PYBIND11_MODULE(ownrecord, m){
    m.def("add", &add, "A function add");
    py::class_<FileReader>(m,"FileReader")
    .def(py::init<const std::string &, bool>())
    .def("printName", &FileReader::printName);

    py::class_<FileWriter>(m,"FileWriter")
    .def(py::init<const std::string &, bool>())
    .def("printName", &FileWriter::printName);
}