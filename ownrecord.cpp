#include "ownrecord.h"



void FileWriter::write_one(py::bytearray data)
{
    std::string str = std::string(data);
    iovData = {(void*) str.c_str(), str.length()};
    // printf("str: %s, len: %d\n", str.c_str(), str.length());

    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_writev(sqe, outfd, &iovData, 1, offset);
    io_uring_sqe_set_data(sqe, iovData.iov_base);
    io_uring_submit(&ring);
    sqe = io_uring_get_sqe(&ring);
    iovIdx = {(void*) &offset, 8};
    io_uring_prep_writev(sqe, outfd, &iovIdx, 1, index_base + 8 * data_count);
    io_uring_sqe_set_data(sqe, iovIdx.iov_base);


    /* waitting for complete */
    offset += str.length();
    data_count += 1;
    // printf("curr size:%d\n", offset);
    io_uring_wait_cqe(&ring, &cqe);
}

FileWriter::FileWriter(const std::string &name, int n): fname(name), N(n){
    // printf("n: %d\n",n);
    outfd = open(name.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    setup_context(QD, &ring);    
    iovMeta = {aligned_alloc(512, 4 + 8 + 4*n + 8*n), (unsigned long) (4 + 8 + 4*n + 8*n)};
    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_writev(sqe, outfd, &iovMeta, 1, 0);
    io_uring_sqe_set_data(sqe, iovMeta.iov_base);
    io_uring_submit(&ring);

    checksum_base = 12;
    index_base = 12 + 4*n;

    io_uring_wait_cqe(&ring, &cqe);
    offset =  (unsigned long long) (4 + 8 + 4*n + 8*n);
    // printf("current %d\n", offset);
}

void FileWriter::close(){
    ::close(outfd);
    io_uring_queue_exit(&ring);
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
    .def(py::init<const std::string &, int>())
    .def("printName", &FileWriter::printName)
    .def("write_one",&FileWriter::write_one)
    .def("close",&FileWriter::close);
}