#include "ownrecord.h"



void FileWriter::write_(ull offset, size_t size, void* data){
    iovBuffer.iov_base = data;
    iovBuffer.iov_len = size;

    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_writev(sqe, outfd, &iovBuffer, 1, offset);
    io_uring_sqe_set_data(sqe, iovBuffer.iov_base);
    io_uring_submit(&ring);

    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

}

void FileWriter::write_one(std::string data)
{

    std::string str = std::string(data);
    iovData = {(void*) str.c_str(), str.length()};

    write_(curr_offset, str.length(), (void*) str.c_str());

    write_(index_base + 8*data_count, 8, &curr_offset);


    /* waitting for complete */
    curr_offset += str.length();
    data_count += 1;

}

FileWriter::FileWriter(const std::string &name, ull n): fname(name), N(n){
    outfd = open(name.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    setup_context(QD, &ring);
    iovMeta = {aligned_alloc(512, 4 + 8 + 4*n + 8*n), (ull) (4 + 8 + 4*n + 8*n)};
    
    memcpy(static_cast<char*>(iovMeta.iov_base) + 4, &n, 8);

    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_writev(sqe, outfd, &iovMeta, 1, 0);
    io_uring_sqe_set_data(sqe, iovMeta.iov_base);
    io_uring_submit(&ring);

    data_count = 0;
    checksum_base = 12;
    index_base = 12 + 4*n;
    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);
    curr_offset = 4 + 8 + 4*n + 8*n;
}

void FileWriter::close(){
    ::close(outfd);
    io_uring_queue_exit(&ring);
}






FileReader::FileReader(const std::string &name,  const bool check_data = true)
    :fname(name), check_data(check_data) {
        buffer = malloc(1024);
        infd = open(name.c_str(), O_RDONLY);
        setup_context(QD, &ring);
        iovIdx = {malloc(8), 8};
        iovec iovN = {malloc(8), 8};

        read_(4, 8);

        n = *(ull*) buffer;
        index_base = 12 + 4*n;

        checksum_base = 12;
}

std::vector<py::bytes> FileReader::read(const std::vector<int>& indices){
    std::vector<py::bytes> result;
    off_t fsize;
    size_t s;
    ull aoffset, boffset, a, b;
    get_file_size(infd, &fsize);
    for(int i = 0; i < indices.size(); i++){

        if(indices[i] == n-1){
            a = index_base + indices[i] * 8;
            read_(a, 8);
            aoffset = *(ull*)buffer;
            s = fsize - aoffset;
        }else{
            a = index_base + indices[i] * 8;
            b = index_base + indices[i]* 8 + 8;
            read_(a, 8);

            aoffset = *(ull*)buffer;
            read_(b, 8);
            boffset = *(ull*)buffer;
            s = boffset - aoffset;
        }

        read_(aoffset, s);

        py::bytes entry = py::bytes((char*) buffer, s);
        result.push_back(entry);
    }
    return result;
}


void FileReader::read_(ull offset, size_t size){

    iovBuffer = {buffer, size};

    sqe = io_uring_get_sqe(&ring);
    io_uring_prep_readv(sqe, infd, &iovBuffer, 1, offset);
    io_uring_sqe_set_data(sqe, iovBuffer.iov_base);
    io_uring_submit(&ring);
    io_uring_wait_cqe(&ring, &cqe);
    io_uring_cqe_seen(&ring, cqe);

}


void FileReader::close(){
    ::close(infd);
    io_uring_queue_exit(&ring);
}



PYBIND11_MODULE(ownrecord, m){
    py::class_<FileReader>(m,"FileReader")
    .def(py::init<const std::string &, const bool >(), py::arg("name") ,py::arg("check_data") = true)
    .def("printName", &FileReader::printName)
    .def("read", &FileReader::read)
    .def("close", &FileReader::close)
    .def_readwrite("n", &FileReader::n);

    py::class_<FileWriter>(m,"FileWriter")
    .def(py::init<const std::string &, ull>())
    .def("printName", &FileWriter::printName)
    .def("write_one",&FileWriter::write_one)
    .def("close",&FileWriter::close);
}

