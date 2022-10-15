cmake_minimum_required(VERSION 3.16)

project(a)

aux_source_directory(. DIR_SRCS)

add_executable(a ${DIR_SRCS})