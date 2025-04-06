add_subdirectory(libs)

foreach (LIB_NAME ${${PROJECT_NAME}_LIBS})
    set(${LIB_NAME}_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${LIB_NAME})
    set(${LIB_NAME}_SOURCE_DIR ${${LIB_NAME}_DIR}/src)
    set(${LIB_NAME}_INCLUDE_DIR ${${LIB_NAME}_DIR}/include)
    set(${LIB_NAME}_TEST_DIR ${${LIB_NAME}_DIR}/tests)
    file(GLOB_RECURSE ${LIB_NAME}_SOURCES ${${LIB_NAME}_SOURCE_DIR}/*.cpp)

    message(STATUS "Adding library ${LIB_NAME}")
    add_library(${LIB_NAME} STATIC ${${LIB_NAME}_SOURCES})
    target_include_directories(${LIB_NAME}
        PUBLIC ${${LIB_NAME}_INCLUDE_DIR}
        PRIVATE ${${LIB_NAME}_INCLUDE_DIR}/${LIB_NAME}_bits/
    )

    # Set the output directory for the library
    set_target_properties(${LIB_NAME} PROPERTIES
        OUTPUT_NAME ${LIB_NAME}
    )
    # Link the library with all needed libraries
    foreach (LIB_LIB ${${PROJECT_NAME}_LIB_LIBS})
        target_link_libraries(${LIB_NAME} PUBLIC ${LIB_LIB})
    endforeach()

    # Link the library with all external libraries
    foreach (EXTERNAL ${${PROJECT_NAME}_EXTERNAL_LIBS})
        get_target_property(EXTERNAL_TYPE ${EXTERNAL} TYPE)
        if (${EXTERNAL_TYPE} STREQUAL "INTERFACE_LIBRARY")
            target_link_libraries(${LIB_NAME} INTERFACE ${EXTERNAL})
        else()
            target_link_libraries(${LIB_NAME} PUBLIC ${EXTERNAL})
        endif()
    endforeach()

    # Testing
    file(GLOB_RECURSE ${LIB_NAME}_TEST_FILES ${${LIB_NAME}_TEST_DIR}/*.cpp) # Each .cpp file in the tests directory is a test
    foreach (TEST_FILE ${${LIB_NAME}_TEST_FILES})
        string(REPLACE ".cpp" "" TEST_NAME ${TEST_FILE}) # Strip the .cpp extension
        string(REPLACE ${${LIB_NAME}_TEST_DIR}/ "" TEST_NAME ${TEST_NAME}) # Strip the library test directory
        set(TEST_NAME ${LIB_NAME}_${TEST_NAME}) # Prepend the library name to the test name

        add_executable(${TEST_NAME} ${TEST_FILE}) # Create an executable for the test
        target_link_libraries(${TEST_NAME} ${LIB_NAME}) # Link the library to the test
        set_target_properties(${TEST_NAME} PROPERTIES
            OUTPUT_NAME ${TEST_NAME}
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_TEST_OUTPUT_DIRECTORY}/${LIB_NAME}
        )
        add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME}) # Add the test to CTest
        set_tests_properties(${TEST_NAME} PROPERTIES
            LABELS ${LIB_NAME} # Set the label for the test
            WORKING_DIRECTORY ${CMAKE_TEST_OUTPUT_DIRECTORY}/${LIB_NAME} # Set the test to run in the same directory as the library
        )
    endforeach()
endforeach()