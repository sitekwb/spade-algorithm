cmake_minimum_required(VERSION 3.20)
project(spade_algorithm)

set(CMAKE_CXX_STANDARD 23)
find_package(Boost 1.65.0 COMPONENTS system filesystem REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(spade_algorithm main.cpp src/SpadeAlgorithm.cpp src/Tidlist.cpp src/Transaction.cpp src/Item.cpp src/SpadeSequence.cpp src/Client.cpp src/SpadeSubseq.cpp)

