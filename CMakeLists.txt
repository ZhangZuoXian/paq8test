cmake_minimum_required(VERSION 3.10)
project(paq8test)

SET(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_CXX_FLAGS "-O3 -floop-strip-mine -funroll-loops -ftree-vectorize -fgcse-sm -falign-loops=16 -ggdb")


include_directories(
${PROJECT_SOURCE_DIR}/src
)

add_executable(paq8test src/map/AdaptiveMap.cpp src/ArithmeticEncoder.cpp src/model/ContextModel.cpp src/map/ContextMap.cpp src/map/ContextMap2.cpp src/Encoder.cpp src/util/Ilog.cpp src/model/IndirectModel.cpp src/model/MatchModel.cpp src/Mixer.cpp src/Models.cpp src/model/NormalModel.cpp src/paq8test.cpp src/Predictor.cpp src/util/ProgramChecker.cpp src/util/Random.cpp src/Shared.cpp src/map/SmallStationaryContextMap.cpp src/map/StateMap.cpp src/map/StateTable.cpp src/map/StationaryMap.cpp src/UpdateBroadcaster.cpp 
src/model/SparseModel.cpp)

