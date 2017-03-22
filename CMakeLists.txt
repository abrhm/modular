PROJECT (test CXX)

SET(CMAKE_CXX_FLAGS "-std=c++11")

ADD_LIBRARY(example SHARED example.cpp)

ADD_EXECUTABLE(test main.cpp)
TARGET_LINK_LIBRARIES(test dl)
