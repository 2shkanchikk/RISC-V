#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "address.h"

static int uart_putc(char c, FILE* file){
    (void)file;
    *UART = c;
    return (unsigned char)c;
}

static FILE uart_file = FDEV_SETUP_STREAM(uart_putc, NULL, NULL, _FDEV_SETUP_WRITE);

FILE* const stdin  = &uart_file;
FILE* const stdout = &uart_file;
FILE* const stderr = &uart_file;

__attribute__((noreturn)) void _exit(int code){
    (void)code;
    __asm__ volatile("ebreak");
    while (1) {}
}

int open(const char* path, int flags, ...){
    *HostFS_ARG0 = (unsigned int)path;
    if(flags & O_APPEND){
        *HostFS_ARG1 = 0x02;
    } else{
        if((flags & O_ACCMODE) == O_RDONLY){
            *HostFS_ARG1 = 0x00;
        } else if((flags & O_ACCMODE) == O_WRONLY){
            *HostFS_ARG1 = 0x01;
        } else if((flags & O_ACCMODE) == O_RDWR){
            *HostFS_ARG1 = 0x03;
        }
    }
    
    *HostFS_CMD = 0x00;
    int res = (int)*HostFS_RESULT;
    if (res < 0) return -1;
    return res + 3;
}

int close(int fd){
    if (fd < 3) return -1;
    *HostFS_ARG0 = fd - 3;
    *HostFS_CMD  = 0x01;
    return *HostFS_RESULT;
}

ssize_t read(int fd, void* buf, size_t count){
    if (fd < 3) return -1;
    *HostFS_ARG0 = fd - 3;
    *HostFS_ARG1 = (unsigned int)buf;
    *HostFS_ARG2 = count;
    *HostFS_CMD  = 0x10;
    return *HostFS_RESULT;
}

off_t lseek(int fd, off_t offset, int whence){
    if (fd < 3) return -1;
    *HostFS_ARG0 = fd - 3;
    *HostFS_ARG1 = offset;
    *HostFS_ARG2 = whence;
    *HostFS_CMD  = 0x11;
    return (int)*HostFS_RESULT;
}

ssize_t write(int fd, const void* buf, size_t count){
    if (fd < 3) return -1;
    *HostFS_ARG0 = fd - 3;
    *HostFS_ARG1 = (unsigned int)buf;
    *HostFS_ARG2 = count;
    *HostFS_CMD  = 0x20;
    return *HostFS_RESULT;
}