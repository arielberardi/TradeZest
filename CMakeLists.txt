cmake_minimum_required(VERSION 3.30.3)
project(TradeZest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


### DEPENDENCIES ###

# Boost (requires boost to be already installed in the workstation)
find_package(Boost REQUIRED COMPONENTS ALL)

include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIRS})

set(BOOST_LIBRARIES
    boost_json
    boost_url
    boost_exception
)

# Google Test Suite
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/b514bdc898e2951020cbdca1304b75f5950d1f59.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(GOOGLE_TEST_LIBRARIES
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
)

include(GoogleTest)

### BUILD PROJECT ####

# add_definitions(-DNDEBUG)

include_directories(${PROJECT_SOURCE_DIR}/Source)
include_directories(${PROJECT_SOURCE_DIR})

set(SOURCES
    Source/HTTPRequest.cpp
    Source/ConfigManager.cpp
)

set(LIBRARIES
    ${BOOST_LIBRARIES}
    ssl
    crypto
)

# Main Build
add_executable(${PROJECT_NAME} ${SOURCES} Source/Main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})

# Copy ca.pem file to binary folder
set(SOURCE_FILE "${CMAKE_SOURCE_DIR}/ca.pem")
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/ca.pem"
            "${CMAKE_BINARY_DIR}/ca.pem"
)

# Copy config.txt file to binary folder
set(SOURCE_FILE "${CMAKE_SOURCE_DIR}/config.txt")
add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/config.txt"
            "${CMAKE_BINARY_DIR}/config.txt"
)


### BUILD TESTS ###
enable_testing()

set(TEST_FILES
    HTTPRequest
    OandaApi
)

foreach(file ${TEST_FILES})
    add_executable(${file}_test ${SOURCES} Tests/${file}Test.cpp)
    target_link_libraries(${file}_test ${GOOGLE_TEST_LIBRARIES} ${LIBRARIES})
    gtest_discover_tests(${file}_test)
endforeach()
