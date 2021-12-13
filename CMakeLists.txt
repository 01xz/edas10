cmake_minimum_required(VERSION 3.13)

set(SUITESPARSE_ROOT "/home/phlexing/SuiteSparse-5.10.1")

project(fieldsolver2d LANGUAGES C)

set(CMAKE_C_STANDARD 99)

##--------------------- Compile Options ------------------------------------##
# Configure compile options
add_compile_options(-Wall -Wextra -pedantic -Werror)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pipe -std=c99")

##--------------------- Global Macros --------------------------------------##
add_definitions(-DDEBUG)

##--------------------- Include directories --------------------------------##
include_directories(
  ${PROJECT_SOURCE_DIR}/include
  ${SUITESPARSE_ROOT}/include
)

##--------------------- Link directories -----------------------------------##
link_directories(
  ${SUITESPARSE_ROOT}/lib
)

##--------------------- Source files ---------------------------------------##
file(GLOB_RECURSE C_FILES
  ${PROJECT_SOURCE_DIR}/src/*.c
)

##--------------------- Build target ---------------------------------------##
# Build executable
add_executable(${PROJECT_NAME} ${C_FILES})
target_link_libraries(${PROJECT_NAME} PRIVATE
  amd
  blas
  camd
  ccolamd
  cholmod
  colamd
  klu
  lapack
  m
  metis
) 
