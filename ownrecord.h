#include <pybind11/pybind11.h>
#include <string>
#include <vector>
#include <linux/io_uring.h>

namespace py = pybind11;


class FileReader
{
public:
    FileReader(const std::string &name, const bool check_data=true): 
    fname(name), check_data(check_data) {}
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
    FileWriter(const std::string &name, const bool check_data=true): 
    fname(name), check_data(check_data) {}
    void printName(){
        printf("print name:%s\n", this->fname.c_str());
    }
    void write_one(const py::bytearray b);
    void close();
    // vector<unsigned

private:
    std::string fname;
    bool check_data;
    
};