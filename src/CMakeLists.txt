file(GLOB_RECURSE paq8test_sources ${PROJECT_SOURCE_DIR}/src/*/*.cpp ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(paq8test_shared SHARED ${paq8test_sources})

target_link_libraries(paq8test_shared)