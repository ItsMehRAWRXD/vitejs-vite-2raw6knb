cmake_minimum_required(VERSION 3.18.1)
project("advancedmediaplayer")

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_library(log-lib log)
find_library(android-lib android)

# Add Kodi native library (will be copied during build)
add_library(kodi-native SHARED IMPORTED)
set_target_properties(kodi-native PROPERTIES
    IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../libs/${ANDROID_ABI}/libkodi.so)

# Build our native library
add_library(advancedmediaplayer SHARED
    main_wrapper.cpp
    kodi_loader.cpp
    diggz_config.cpp
    knox_evasion.cpp
    zip_extractor.cpp)

# Include directories
target_include_directories(advancedmediaplayer PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(advancedmediaplayer
    ${android-lib}
    ${log-lib}
    kodi-native
    z)  # zlib for zip extraction

# Compiler flags for optimization and obfuscation
target_compile_options(advancedmediaplayer PRIVATE
    -O3
    -fvisibility=hidden
    -ffunction-sections
    -fdata-sections
    -fstack-protector-strong)