cmake_minimum_required(VERSION 3.5) 

project(wildfire_analysis)

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)
#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
find_package(PostgreSQL REQUIRED)
include_directories (${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

add_executable(main src/main.cpp src/Cell.cpp src/CellStorage.cpp src/Connection.cpp src/Coordinates.cpp src/Fire.cpp src/Wind.cpp src/Math.cpp src/Metric.cpp)

#add_test(test_all "./tests")

target_link_libraries(main)