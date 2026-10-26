set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR riscv32)
set(CMAKE_C_COMPILER riscv64-unknown-elf-gcc)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(CMAKE_C_FLAGS_INIT "-march=rv32im -mabi=ilp32 --specs=picolibc.specs")

set(CMAKE_EXE_LINKER_FLAGS_INIT
    "-Wl,--defsym=__flash=0x80000000 -Wl,--defsym=__flash_size=0x00800000 \
     -Wl,--defsym=__ram=0x80800000   -Wl,--defsym=__ram_size=0x01800000 \
     -Wl,--defsym=__stack_size=0x10000")