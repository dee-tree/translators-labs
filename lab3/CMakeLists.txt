cmake_minimum_required(VERSION 3.23)
project(labs C)

set(CMAKE_C_STANDARD 99)

set(SOURCE_EXE lab3/c/parser/parse_0.c)		# Установка переменной со списком исходников для исполняемого файла

set(MEMSTRUCT_LIB lab3/c/parser/memStruct.c)
set(PARSER_LIB lab3/c/parser/parser.c)
set(SCANNER_LIB lab3/c/parser/scanner.c)

add_library(memstruct STATIC ${MEMSTRUCT_LIB})
add_library(scanner STATIC ${SCANNER_LIB})
add_library(parser STATIC ${PARSER_LIB})

add_executable(main lab3/c/parser/parse_0.c)

target_link_libraries(main memstruct scanner parser)