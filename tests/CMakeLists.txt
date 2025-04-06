enable_testing()

foreach(TEST_NAME ${${PROJECT_NAME}_TESTS})
    message(STATUS "Configuring test ${TEST_NAME}")
    set(${TEST_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${TEST_NAME})
    file(GLOB_RECURSE ${TEST_NAME}_SOURCES ${${TEST_NAME}_SOURCE_DIR}/*.cpp)
    file(GLOB_RECURSE ${TEST_NAME}_HEADERS ${${TEST_NAME}_SOURCE_DIR}/*.hpp)
    add_executable(${TEST_NAME} ${${TEST_NAME}_SOURCES} ${${TEST_NAME}_HEADERS})
    target_include_directories(${TEST_NAME} PUBLIC ${${TEST_NAME}_SOURCE_DIR})

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

    # Link the library with all previously created libraries
    foreach (LIB ${${PROJECT_NAME}_LIBS} ${${PROJECT_NAME}_LIB_LIBS})
        get_target_property(LIB_TYPE ${LIB} TYPE)
        if (${LIB_TYPE} STREQUAL "INTERFACE_LIBRARY")
            target_link_libraries(${TEST_NAME} INTERFACE ${LIB})
        else()
            target_link_libraries(${TEST_NAME} PUBLIC ${LIB})
        endif()
    endforeach()

    # Link the library with all external libraries
    foreach (EXTERNAL ${${PROJECT_NAME}_EXTERNAL_LIBS})
        get_target_property(EXTERNAL_TYPE ${EXTERNAL} TYPE)
        if (${EXTERNAL_TYPE} STREQUAL "INTERFACE_LIBRARY")
            target_link_libraries(${TEST_NAME} INTERFACE ${EXTERNAL})
        else()
            target_link_libraries(${TEST_NAME} PUBLIC ${EXTERNAL})
        endif()
    endforeach()
endforeach()