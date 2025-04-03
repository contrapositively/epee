enable_testing()

foreach(TEST_NAME ${${PROJECT_NAME}_TESTS})
    message(STATUS "Configuring test ${TEST_NAME}")
    set(${TEST_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME})
    file(GLOB_RECURSE ${TEST_NAME}_SOURCES ${${TEST_NAME}_SOURCE_DIR}/*.cpp)
    file(GLOB_RECURSE ${TEST_NAME}_HEADERS ${${TEST_NAME}_SOURCE_DIR}/*.hpp)
    add_executable(${TEST_NAME} ${${TEST_NAME}_SOURCES} ${${TEST_NAME}_HEADERS})
    target_include_directories(${TEST_NAME} PUBLIC ${${TEST_NAME}_SOURCE_DIR})
    target_link_libraries(${TEST_NAME} ${${PROJECT_NAME}_LIBS})

    # Set the output directory for the test
    set_target_properties(${TEST_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_TEST_OUTPUT_DIRECTORY}
        OUTPUT_NAME ${TEST_NAME}
    )

    # Add the test to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 10
        WORKING_DIRECTORY ${CMAKE_TEST_OUTPUT_DIRECTORY}
    )
endforeach()