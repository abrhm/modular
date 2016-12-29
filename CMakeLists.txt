PROJECT (modular)

SET(CMAKE_CXX_FLAGS "-std=c++11 -ldl -Wall")

ADD_LIBRARY(example1 SHARED example1.cpp)
ADD_LIBRARY(example2 SHARED example2.cpp)

ADD_EXECUTABLE(example main.cpp)
TARGET_LINK_LIBRARIES(example dl)
