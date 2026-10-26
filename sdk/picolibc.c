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

