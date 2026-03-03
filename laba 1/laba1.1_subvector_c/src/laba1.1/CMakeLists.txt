get_filename_component(LAB_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(EXECUTABLE_NAME ${LAB_NAME})

file(GLOB LAB_SOURCES "*.cpp")

add_executable(${EXECUTABLE_NAME} ${LAB_SOURCES})

target_include_directories(${EXECUTABLE_NAME} 
    PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${PROJECT_SOURCE_DIR}/include/${LAB_NAME}
)

set_target_properties(${EXECUTABLE_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "Configuring ${LAB_NAME}")
message(STATUS "  Sources: ${LAB_SOURCES}")