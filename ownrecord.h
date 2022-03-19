#include <pybind11/pybind11.h>
#include <string>
#include <vector>
#include "io_utils.h"


// #include "io_utils.h"

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
    FileWriter(const std::string &name, int n);
    void printName(){
        printf("print name:%s\n", this->fname.c_str());
    }
    void write_one(const py::bytearray b);
    void close();

private:

    unsigned long long get_index_offset(int index);

    int outfd;
    io_uring_sqe *sqe;
    io_uring_cqe *cqe;
    io_uring ring;
    iovec iovMeta;
    iovec iovData;
    iovec iovIdx;
    
    unsigned long long offset;
    unsigned long long index_base;
    unsigned long long checksum_base;
    std::string fname;

    int N;
    int data_count;
    bool check_data;
    
};