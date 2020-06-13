cmake_minimum_required(VERSION 2.6)
project(mWebSockets)

include_directories("${CMAKE_CURRENT_LIST_DIR}/src")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/base64")
include_directories("${CMAKE_CURRENT_LIST_DIR}/src/CryptoLegacy")
add_subdirectory(examples)
