cmake_minimum_required(VERSION 3.6)
project(echo-server)

file(GLOB source
    "src/*.cpp"
    "src/*.c"
)
add_executable(echo-server ${source})

# include directories with header files (Same for Windows and Linux)
include_directories(src/include)

# link against static libraries 
# (Linux)
# target_link_libraries(LearnOpenGL ${CMAKE_SOURCE_DIR}/src/lib/libglfw3.a)