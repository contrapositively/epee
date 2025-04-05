foreach(EXEC_NAME ${${PROJECT_NAME}_EXECS})
    message(STATUS "Configuring executable ${EXEC_NAME}")
    set(${EXEC_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${EXEC_NAME})
    file(GLOB_RECURSE ${EXEC_NAME}_SOURCES ${${EXEC_NAME}_SOURCE_DIR}/*.cpp)
    file(GLOB_RECURSE ${EXEC_NAME}_HEADERS ${${EXEC_NAME}_SOURCE_DIR}/*.hpp)
    add_executable(${EXEC_NAME} ${${EXEC_NAME}_SOURCES} ${${EXEC_NAME}_HEADERS})
    target_include_directories(${EXEC_NAME} PUBLIC ${${EXEC_NAME}_SOURCE_DIR})
    
    # Set the output directory for the executable
    set_target_properties(${EXEC_NAME} PROPERTIES
        OUTPUT_NAME ${EXEC_NAME}
    )
    
    # Link the library with all previously created libraries
    foreach (LIB ${${PROJECT_NAME}_LIBS})
        get_target_property(LIB_TYPE ${LIB} TYPE)
        if (${LIB_TYPE} STREQUAL "INTERFACE_LIBRARY")
            target_link_libraries(${EXEC_NAME} INTERFACE ${LIB})
        else()
            target_link_libraries(${EXEC_NAME} PUBLIC ${LIB})
        endif()
    endforeach()

    # Link the library with all external libraries
    foreach (EXTERNAL ${${PROJECT_NAME}_EXTERNAL_LIBS})
        get_target_property(EXTERNAL_TYPE ${EXTERNAL} TYPE)
        if (${EXTERNAL_TYPE} STREQUAL "INTERFACE_LIBRARY")
            target_link_libraries(${EXEC_NAME} INTERFACE ${EXTERNAL})
        else()
            target_link_libraries(${EXEC_NAME} PUBLIC ${EXTERNAL})
        endif()
    endforeach()
endforeach()
