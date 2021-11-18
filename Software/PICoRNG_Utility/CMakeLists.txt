cmake_minimum_required(VERSION 3.14)
project(PICoRNG_Utility)

set(CMAKE_CXX_STANDARD 17)

set(CPM_DOWNLOAD_VERSION 0.27.5)
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM_${CPM_DOWNLOAD_VERSION}.cmake")

if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD https://github.com/TheLartians/CPM.cmake/releases/download/v${CPM_DOWNLOAD_VERSION}/CPM.cmake ${CPM_DOWNLOAD_LOCATION})
endif()

include(${CPM_DOWNLOAD_LOCATION})

CPMAddPackage(
        NAME cxxopts
        GITHUB_REPOSITORY SudoMaker/cxxopts
        VERSION 3.0.0
        GIT_TAG 5eca8a30012b69b76316b71fa391a89fe09256cb
        GIT_SHALLOW ON
)

CPMAddPackage(
        NAME EntropyCheck
        GITHUB_REPOSITORY SudoMaker/EntropyCheck
        VERSION 0.0.0
        GIT_SHALLOW ON
)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/CMake)

find_package(LibUSB)
include_directories(${LibUSB_INCLUDE_DIRS})

add_executable(PICoRNG
        tinyECDH/ecdh.c tinyECDH/ecdh.h
        CommonIncludes.hpp
        main.cpp
        Components/USB.hpp Components/USB.cpp Components/Protocol.h
        Commands/Commands.hpp Commands/Cat.cpp Commands/Quality.cpp Commands/ListDevs.cpp
        Components/Verification.cpp Components/Verification.hpp Components/Utilities.cpp Components/Utilities.hpp Commands/Info.cpp Commands/Pair.cpp Commands/Verify.cpp Commands/RngD.cpp)
target_link_libraries(PICoRNG cxxopts EntropyCheck ${LibUSB_LIBRARIES})

if(UNIX AND NOT APPLE)
    target_link_libraries(PICoRNG stdc++fs)
endif()