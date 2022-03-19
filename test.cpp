#include <linux/io_uring.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/uio.h>
#include <linux/fs.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>



// int io_uring_setup(unsigned entries, struct io_uring_params *p)
// {
//     return (int) syscall(__NR_io_uring_setup, entries, p);
// }

// int io_uring_enter(int ring_fd, unsigned int to_submit,
//                           unsigned int min_complete, unsigned int flags)
// {
//     return (int) syscall(__NR_io_uring_enter, ring_fd, to_submit, min_complete,
//                    flags, NULL, 0);
// }


// off_t get_file_size(int fd){
//     struct stat st;

//     if(fstat(fd, &st) < 0){
//         perror("fstat");
//         return -1;
//     }
//     if( S_ISBLK(st.st_mode)){
//         unsigned long long bytes;
//         if(ioctl(fd, BLKGETSIZE64, &bytes) != 0){
//             perror("ioctl");
//             return -1;
//         }
//         return bytes;
//     }else if (S_ISREG(st.st_mode))
//         return st.st_size;

//     return -1;
// }

int main()
{
    SYS_io_uring_setup()
}