cmake_minimum_required(VERSION 3.0)
set (CMAKE_CXX_STANDARD 17)

set(PROJECT_NAME "GUI-test")

project(${PROJECT_NAME})

# "Dear ImGui" dependency
set(IMGUI_DIR external/imgui)
set(IMGUI_SRC ${IMGUI_DIR}/imgui.cpp ${IMGUI_DIR}/imgui_demo.cpp ${IMGUI_DIR}/imgui_draw.cpp ${IMGUI_DIR}/imgui_tables.cpp ${IMGUI_DIR}/imgui_widgets.cpp)
set(IMGUI_SRC ${IMGUI_SRC} ${IMGUI_DIR}/backends/imgui_impl_dx9.cpp ${IMGUI_DIR}/backends/imgui_impl_win32.cpp)
include_directories(include ${IMGUI_DIR} ${IMGUI_DIR}/backends)

# "stb" dependency
set(STB_DIR external/stb)
include_directories(include ${STB_DIR})

# Find our own source files
aux_source_directory(src SRC)

# Create the executable file
add_executable(${PROJECT_NAME} ${SRC} ${IMGUI_SRC} main.cpp)
target_link_libraries(${PROJECT_NAME} d3d9.lib) 

# Copy the resources directory
if (EXISTS ${CMAKE_SOURCE_DIR}/res)
  #TODO: copy with overwrite!
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)
endif()
