cmake_minimum_required(VERSION 3.30.3)
project(TradeZest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


### DEPENDENCIES ###

# Boost
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

include_directories(${PROJECT_SOURCE_DIR}/Source)

set(SOURCES
    Source/Broker.cpp
    Source/HTTPRequest.cpp
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


### BUILD TESTS ###
enable_testing()

add_executable(Main_test ${SOURCES} Tests/Main.cpp)
target_link_libraries(Main_test ${GOOGLE_TEST_LIBRARIES} ${LIBRARIES})

gtest_discover_tests(Main_test)
