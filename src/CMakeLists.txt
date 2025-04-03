foreach(EXEC_NAME ${${PROJECT_NAME}_EXECS})
    message(STATUS "Configuring executable ${EXEC_NAME}")
    set(${EXEC_NAME}_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/${EXEC_NAME})
    file(GLOB_RECURSE ${EXEC_NAME}_SOURCES ${${EXEC_NAME}_SOURCE_DIR}/*.cpp)
    file(GLOB_RECURSE ${EXEC_NAME}_HEADERS ${${EXEC_NAME}_SOURCE_DIR}/*.hpp)
    add_executable(${EXEC_NAME} ${${EXEC_NAME}_SOURCES} ${${EXEC_NAME}_HEADERS})
    target_include_directories(${EXEC_NAME} PUBLIC ${${EXEC_NAME}_SOURCE_DIR})
    target_link_libraries(${EXEC_NAME} ${${PROJECT_NAME}_LIBS})
    
    # Set the output directory for the executable
    set_target_properties(${EXEC_NAME} PROPERTIES
        OUTPUT_NAME ${EXEC_NAME}
    )
    
endforeach()
