#include "io_utils.h"
#include <string>


int main(int argc, char *argv[]) {
    struct io_uring ring;
    off_t insize;
    int ret;

    // if (argc < 3) {
    //     printf("Usage: %s <infile> <outfile>\n", argv[0]);
    //     return 1;
    // }

    infd = open(argv[1], O_RDONLY);
    if (infd < 0) {
        perror("open infile");
        return 1;
    }

    // outfd = open(argv[1], O_RDWR | O_CREAT | O_TRUNC, 0644);
    // if (outfd < 0) {
    //     perror("open outfile");
    //     return 1;
    // }

    if (setup_context(QD, &ring))
        return 1;

    // if (get_file_size(infd, &insize))
    //     return 1;

    // io_data *data = (io_data*) malloc(sizeof(io_data));
    // data->read = 0;
    // data->first_offset = 0;
    // data->first_len = 0;
    // data->offset = 0;

    // const char* f = std::string("12345678").c_str();
    // printf("%p\n", f);

    // iovec iov = {(void*)f+8, 4};
    // printf("%p\n",iov.iov_base);

    void* buf = aligned_alloc(512, 1024);

    iovec iov = {buf, 800};

    // data->iov.iov_base = f;
    // data->iov.iov_len = 4*sizeof(char);
    io_uring_sqe *sqe;
    io_uring_cqe* cqe;
    char *data;

    sqe = io_uring_get_sqe(&ring);
    // io_uring_prep_writev(sqe, outfd, &iov, 1, 0);
    // io_uring_sqe_set_data(sqe, iov.iov_base);
    // queue_prepped(&ring, data);


    // io_uring_submit(&ring);


    io_uring_prep_readv(sqe, infd, &iov, 1, 4);
    io_uring_sqe_set_data(sqe, iov.iov_base);
    // io_uring_submit(&ring);


    // for(int i = 0; i < 8; i++){
    //     printf("%c",f[i]);
    // }
    // iov.iov_base -= 4;
    // iov.iov_len = 4;
    // printf("%p\n",iov.iov_base);

    // sqe = io_uring_get_sqe(&ring);
    // io_uring_prep_writev(sqe, outfd, &iov, 1, 8);//4 indicates the offset to the start of the file
    // // io_uring_sqe_set_data(sqe, iov.iov_base);
    // io_uring_sqe_set_data64(sqe, (unsigned long long) iov.iov_base);

    // queue_prepped(&ring, data);


    io_uring_submit(&ring);

    io_uring_wait_cqe(&ring, &cqe);

    data = (char *)io_uring_cqe_get_data(cqe);

    for(int i = 0; i < 4; i++){
        printf("%c",data[i]);
    }


    // int count = 2;
    // while(count > 0){
    //     io_uring_wait_cqe(&ring, &cqe);
    //     count--;
    // }


    // ret = copy_file(&ring, insize);

    // close(infd);
    close(outfd);
    io_uring_queue_exit(&ring);
    return ret;
}