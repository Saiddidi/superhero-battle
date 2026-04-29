cmake_minimum_required(VERSION 3.16)
project(SuperheroBattleSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ── Find nlohmann/json ────────────────────────────────────────
# Arch Linux: sudo pacman -S nlohmann-json
# Ubuntu:     sudo apt install nlohmann-json3-dev
find_package(nlohmann_json QUIET)

if(NOT nlohmann_json_FOUND)
    # Fallback: download automatically
    include(FetchContent)
    FetchContent_Declare(
        nlohmann_json
        URL https://github.com/nlohmann/json/releases/download/v3.11.3/json.hpp
        DOWNLOAD_NO_EXTRACT TRUE
        DOWNLOAD_DIR ${CMAKE_BINARY_DIR}/include/nlohmann
    )
    FetchContent_MakeAvailable(nlohmann_json)
    message(STATUS "nlohmann/json downloaded automatically")
else()
    message(STATUS "nlohmann/json found via system package")
endif()

# ── Source files ──────────────────────────────────────────────
add_executable(SuperheroBattle
    main.cpp
    BattleEngine.cpp
    UI.cpp
)

# ── Link nlohmann/json ────────────────────────────────────────
if(nlohmann_json_FOUND)
    target_link_libraries(SuperheroBattle PRIVATE nlohmann_json::nlohmann_json)
else()
    target_include_directories(SuperheroBattle PRIVATE
        ${CMAKE_BINARY_DIR}/include
    )
endif()

# ── Compiler options ──────────────────────────────────────────
if(MSVC)
    target_compile_options(SuperheroBattle PRIVATE /W4 /O2)
else()
    target_compile_options(SuperheroBattle PRIVATE -Wall -Wextra -O2)
endif()

# ── Copy characters.json to build directory ───────────────────
configure_file(
    ${CMAKE_SOURCE_DIR}/characters.json
    ${CMAKE_BINARY_DIR}/characters.json
    COPYONLY
)

message(STATUS "✅ Project configured. Run: cmake --build .")
