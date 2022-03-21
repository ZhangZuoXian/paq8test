cmake_minimum_required(VERSION 3.10)
project(paq8test)

SET(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "-O3 -floop-strip-mine -funroll-loops -ftree-vectorize -fgcse-sm -falign-loops=16 -ggdb -std=c++17 -L /lib/ -lhiredis ")

######################################################################################################################
# google test
######################################################################################################################

configure_file("${PROJECT_SOURCE_DIR}/build_support/gtest_CMakeLists.txt.in" googletest-download/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/googletest-download")
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)  # don't override our compiler/linker options when building gtest
add_subdirectory("${CMAKE_BINARY_DIR}/googletest-src" "${CMAKE_BINARY_DIR}/googletest-build")


include_directories( ${PROJECT_SOURCE_DIR}/src/include)
include_directories(BEFORE src) # This is needed for gtest.

add_subdirectory(src)
add_subdirectory(test)

add_executable(paq8test src/map/AdaptiveMap.cpp src/ArithmeticEncoder.cpp src/model/ContextModel.cpp src/map/ContextMap.cpp src/map/ContextMap2.cpp src/Encoder.cpp src/util/Ilog.cpp src/model/IndirectModel.cpp src/model/MatchModel.cpp src/Mixer.cpp src/Models.cpp src/model/NormalModel.cpp src/paq8test.cpp src/Predictor.cpp src/util/ProgramChecker.cpp src/util/Random.cpp src/Shared.cpp src/map/SmallStationaryContextMap.cpp src/map/StateMap.cpp src/map/StateTable.cpp src/map/StationaryMap.cpp src/UpdateBroadcaster.cpp 
src/model/SparseModel.cpp src/Statistic.cpp src/util/redisHandler.cpp)

target_link_libraries(paq8test /lib/libhiredis.so.1.0.3-dev)
