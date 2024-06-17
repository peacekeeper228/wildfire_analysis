cmake_minimum_required(VERSION 3.5) 

project(wildfire_analysis)

include_directories(include)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED on)

#set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror")
find_package(PostgreSQL REQUIRED)
include_directories (${PostgreSQL_INCLUDE_DIRS})
link_directories(${PostgreSQL_LIBRARY_DIRS})

ADD_LIBRARY(LibsModule 
    src/Cell.cpp
    src/CellStorage.cpp
    src/Connection.cpp
    src/Coordinates.cpp
    src/Fire.cpp
    src/Wind.cpp
    src/Math.cpp
    src/Metric.cpp
)

target_link_libraries(LibsModule PRIVATE PostgreSQL::PostgreSQL)

ADD_EXECUTABLE(main src/main.cpp)
target_link_libraries(main LibsModule)

SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )

SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/bin
        CACHE PATH
        "Single Directory for all"
    )

SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        ${PROJECT_BINARY_DIR}/lib
        CACHE PATH
        "Single Directory for all"
    )

#include_directories(include)
#add_executable(main main.cpp Cell.cpp CellStorage.cpp Connection.cpp Coordinates.cpp Fire.cpp Wind.cpp Math.cpp Metric.cpp)
