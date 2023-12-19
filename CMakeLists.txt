cmake_minimum_required(VERSION 3.1)
project(Vyrio CXX)

add_library(vyrio SHARED ./src/ichannel.cpp ./src/ochannel.cpp ./src/exception.cpp)
target_compile_features(vyrio PRIVATE cxx_std_11)
target_include_directories(vyrio PRIVATE include)

install(TARGETS vyrio RUNTIME DESTINATION bin LIBRARY DESTINATION lib)
