file(GLOB_RECURSE paq8_sources ${PROJECT_SOURCE_DIR}/src/*/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(paq8_shared SHARED ${paq8_sources})

target_link_libraries(paq8_shared)