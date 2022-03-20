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
    // printf("str: %s, len: %d\n", data.c_str(), data.length());

    write_(curr_offset, str.length(), (void*) str.c_str());
    // sqe = io_uring_get_sqe(&ring);
    // io_uring_prep_writev(sqe, outfd, &iovData, 1, offset);
    // io_uring_sqe_set_data(sqe, iovData.iov_base);
    // io_uring_submit(&ring);
    printf("data %d offset at %lld\n", data_count, curr_offset);
    write_(index_base + 8*data_count, 8, &curr_offset);


    // sqe = io_uring_get_sqe(&ring);
    // iovIdx = {(void*) &curr_offset, 8};
    // io_uring_prep_writev(sqe, outfd, &iovIdx, 1, index_base + 8 * data_count);
    // printf("idx offset:%d\n", index_base+ 8 * data_count);
    // printf("data offset:%lld\n", curr_offset);
    // io_uring_sqe_set_data(sqe, iovIdx.iov_base);
    // io_uring_submit(&ring);


    /* waitting for complete */
    curr_offset += str.length();
    data_count += 1;
    // printf("curr size:%d\n", offset);
    // io_uring_wait_cqe(&ring, &cqe);
    // io_uring_cqe_seen(&ring, cqe);

}

FileWriter::FileWriter(const std::string &name, ull n): fname(name), N(n){
    // printf("n: %lld\n", n);
    outfd = open(name.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
    setup_context(QD, &ring);
    iovMeta = {aligned_alloc(512, 4 + 8 + 4*n + 8*n), (ull) (4 + 8 + 4*n + 8*n)};
    
    memcpy(iovMeta.iov_base + 4, &n, 8);

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
    // printf("current %d\n", index_base);
}

void FileWriter::close(){
    ::close(outfd);
    io_uring_queue_exit(&ring);
}




int add(int i, int j){
    return i+j;
}






FileReader::FileReader(const std::string &name,  const bool check_data = true)
    :fname(name), check_data(check_data) {
        buffer = malloc(1024);
        infd = open(name.c_str(), O_RDONLY);
        setup_context(QD, &ring);
        iovIdx = {malloc(8), 8};
        iovec iovN = {malloc(8), 8};


        // sqe = io_uring_get_sqe(&ring);
        // io_uring_prep_readv(sqe, infd, &iovN, 1, 4);
        // io_uring_sqe_set_data(sqe, iovN.iov_base);

        // io_uring_submit(&ring);

        // io_uring_wait_cqe(&ring, &cqe);


        read_(4, 8);

        // ull *iovd = (ull*) iovN.iov_base;

        // ull* ndata = (ull*)io_uring_cqe_get_data(cqe);
        n = *(ull*) buffer;
        printf("IOV: %lld\n", n);
        index_base = 12 + 4*n;
        printf("idx base: %lld\n", index_base);

        checksum_base = 12;
}

std::vector<py::bytes> FileReader::read(const std::vector<int>& indices){
    std::vector<py::bytes> result;
    off_t fsize;
    size_t s;
    ull aoffset, boffset, a, b;
    get_file_size(infd, &fsize);
    printf("file size %ld\n", fsize);
    for(int i = 0; i < indices.size(); i++){
        printf("index:%d\n", indices[i]);

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
        printf("offset idx a:%lld, b:%lld\n", a, b);

        printf("offset a:%lld, b:%lld\n", aoffset, boffset);
        printf("size: %lld\n", s);
        py::bytes entry = py::bytes((char*) buffer, s);
        result.push_back(entry);
    }
    printf("return %d samples\n", result.size());
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
    m.def("add", &add, "A function add");
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

    pybind11::module_ torch = m.def_submodule("torch");
    py::class_<ownDataset, FileReader>(torch, "Dataset")
    .def(py::init<const std::string &>())
    .def("__getitem__", &ownDataset::__getitem__);

}

