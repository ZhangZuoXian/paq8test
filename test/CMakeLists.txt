file(GLOB PAQ8_TEST_SOURCES "${PROJECT_SOURCE_DIR}/test/*/*test.cpp")

##########################################
# "make check-tests"
##########################################
add_custom_target(build-tests COMMAND ${CMAKE_CTEST_COMMAND} --show-only)
add_custom_target(check-tests COMMAND ${CMAKE_CTEST_COMMAND} --verbose)


##########################################
# "make XYZ_test"
##########################################
foreach (paq8_test_source ${PAQ8_TEST_SOURCES})
    # Create a human readable name.
    get_filename_component(paq8_test_filename ${paq8_test_source} NAME)
    string(REPLACE ".cpp" "" paq8_test_name ${paq8_test_filename})

    # Add the test target separately and as part of "make check-tests".
    add_executable(${paq8_test_name} EXCLUDE_FROM_ALL ${paq8_test_source})
    add_dependencies(build-tests ${paq8_test_name})
    add_dependencies(check-tests ${paq8_test_name})

    target_link_libraries(${paq8_test_name} paq8_shared gtest gmock_main)

    # Set test target properties and dependencies.
    set_target_properties(${paq8_test_name}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/test"
        COMMAND ${paq8_test_name}
    )

    # Add the test under CTest.
    add_test(${paq8_test_name} ${VALGRIND_BIN} ${VALGRIND_OPTIONS}
    --suppressions=${VALGRIND_SUPPRESSIONS_FILE} ${CMAKE_BINARY_DIR}/test/${paq8_test_name}
    --gtest_color=yes --gtest_output=xml:${CMAKE_BINARY_DIR}/test/unit_${test_name}.xml)
    add_test(${paq8_test_name} ${CMAKE_BINARY_DIR}/test/${paq8_test_name} --gtest_color=yes
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test/${paq8_test_name}.xml)
endforeach(paq8_test_source ${paq8_TEST_SOURCES})