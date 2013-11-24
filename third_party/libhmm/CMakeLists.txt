cmake_minimum_required(VERSION 2.8.12)
project("LibHMM")
include_directories(.)
AUX_SOURCE_DIRECTORY(. Libhmm_SRC_LIST)
#AUX_SOURCE_DIRECTORY(./vb Libhmm_SRC_LIST)

AUX_SOURCE_DIRECTORY(./newmat Libhmm_SRC_LIST)
add_library(libhmm ${Libhmm_SRC_LIST})
