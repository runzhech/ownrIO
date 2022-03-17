#include <pybind11/pybind11.h>
#include <string>
#include <vector>
namespace py = pybind11;

class FileReader
{
public:
    FileReader(const std::string &name, const bool check_data=True): 
    name(name), check_data(check_data) {}
    void printName(){
        printf("print name:%s\n", this->fname.c_str());
    }
    
    // vector<unsigned

private:
    std::string fname;
    bool check_data;
    
};

class FileWriter
{
public:
    FileWriter(const std::string &name, const bool check_data=True): 
    name(name), check_data(check_data) {}
    void printName(){
        printf("print name:%s\n", this->fname.c_str());
    }
    void write_one(const bytearray b);
    // vector<unsigned

private:
    std::string fname;
    bool check_data;
    
};


int add(int i, int j){
    return i+j;
}

PYBIND11_MODULE(ownrecord, m){
    m.def("add", &add, "A function add");
    py::class_<FileReader>(m,"FileReader")
    .def(py::init<const std::string &>())
    .def("printName", &FileReader::printName);
}