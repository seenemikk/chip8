cmake_minimum_required(VERSION 3.17)
project(chip8 C)

set(CMAKE_C_STANDARD 99)

add_executable(chip8 src/main.c src/inc/chip8.h src/chip8_display.c src/inc/chip8_display.h src/chip8_stack.c src/inc/chip8_stack.h src/chip8_memory.c src/inc/chip8_memory.h src/chip8_registers.c src/inc/chip8_registers.h src/chip8_keyboard.c src/inc/chip8_keyboard.h src/chip8.c)

INCLUDE(FindPkgConfig)

PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})