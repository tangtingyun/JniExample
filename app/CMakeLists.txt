cmake_minimum_required(VERSION 3.4.1)
find_library(jnigraphics-lib
        jnigraphics)
set(distribution_DIR ../../../../libs)

add_library(luban
        SHARED
        IMPORTED)
set_target_properties(luban
        PROPERTIES IMPORTED_LOCATION
        ${distribution_DIR}/${ANDROID_ABI}/libluban.so)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")


add_library(
        native-lib
        SHARED
        src/main/cpp/native-lib.cpp
        src/main/cpp/md5.cpp)

include_directories(src/main/cpp)
find_library(
        log-lib
        log)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
target_link_libraries(
        native-lib
        luban
        ${log-lib}
        ${jnigraphics-lib}
)
