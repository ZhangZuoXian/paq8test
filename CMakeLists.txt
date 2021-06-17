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

add_executable(paq8test src/AdaptiveMap.cpp src/ArithmeticEncoder.cpp src/ContextModel.cpp src/ContextMap.cpp src/ContextMap2.cpp src/Encoder.cpp src/Ilog.cpp src/IndirectModel.cpp src/MatchModel.cpp src/Mixer.cpp src/Models.cpp src/NormalModel.cpp src/paq8test.cpp src/Predictor.cpp src/ProgramChecker.cpp src/Random.cpp src/Shared.cpp src/SmallStationaryContextMap.cpp src/StateMap.cpp src/StateTable.cpp src/StationaryMap.cpp src/UpdateBroadcaster.cpp )

