#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <string>
#include <vector>
#include "io_utils.h"


// #include "io_utils.h"

namespace py = pybind11;


class FileReader
{
public:
    FileReader(const std::string &name, const bool check_data);
    void printName(){
        printf("print name:%s\n", this->fname.c_str());
    }

    std::vector<py::bytes> read(const std::vector<int> &indices);

    void close();
    ull n;


private:
    void read_(ull offset, size_t size);
    std::string fname;
    bool check_data;
    void* buffer;
    int infd;
    io_uring_sqe *sqe;
    io_uring_cqe *cqe;
    io_uring ring;
    iovec iovMeta;
    iovec iovData;
    iovec iovIdx;
    iovec iovBuffer;
    ull offset;
    ull index_base;
    ull checksum_base;
};


class FileWriter
{
public:
    FileWriter(const std::string &name, ull n);
    void printName(){
        printf("print name:%s\n", this->fname.c_str());
    }
    void write_one(const std::string b);
    void close();

private:

    ull get_index_offset(int index);
    void write_(ull offset, size_t size, void* data);
    int outfd;
    void* buffer;
    io_uring_sqe *sqe;
    io_uring_cqe *cqe;
    io_uring ring;
    iovec iovMeta;
    iovec iovData;
    iovec iovIdx;
    iovec iovBuffer;
    ull curr_offset;
    ull index_base;
    ull checksum_base;
    std::string fname;

    ull N;
    int data_count;
    bool check_data;
    
};

