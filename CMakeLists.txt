cmake_minimum_required(VERSION 3.10)
project(SoftwareRasterizeRenderer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${PROJECT_SOURCE_DIR}/include)

set(SOURCES
        "src/geometry.cpp"
        "src/Graphic_Library.cpp"
        "src/main.cpp"
        "src/model.cpp"
        "src/Test.cpp"
        "src/tgaimage.cpp"
)

add_executable(SoftwareRasterizeRenderer ${SOURCES})

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_custom_command(TARGET SoftwareRasterizeRenderer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/obj
        $<TARGET_FILE_DIR:SoftwareRasterizeRenderer>/obj
)