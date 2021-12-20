set (CMAKE_SYSTEM_PROCESSOR "arm" CACHE STRING "")
set (MACHINE                "a926" CACHE STRING "")
set (CROSS_PREFIX           "arm-none-eabi-" CACHE STRING "")
set (CMAKE_C_FLAGS          "-mcpu=arm926ej-s -mfloat-abi=soft" CACHE STRING "")
set (LIBMETAL_INCLUDE_DIR   "../bb/lib/include" CACHE STRING "")
set (LIBMETAL_LIB           "../bb/lib/libmetal.a" CACHE STRING "")
include (cross_generic_gcc)

# vim: expandtab:ts=2:sw=2:smartindent
