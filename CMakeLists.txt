cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(SimplyEmail LANGUAGES CXX VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(CURL REQUIRED)

list(APPEND CXX_FLAGS "-Wall" "-Wextra" "-Werror" "-pedantic" "-ansi")

add_library(simplyemail
    STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/Email.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/EmailAttachment.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/SMTPConnection.cpp)

target_include_directories(simplyemail
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/lib
	${CURL_INCLUDE_DIRS})

target_link_libraries(simplyemail
    PRIVATE
        ${CURL_LIBRARIES})
	
target_compile_options(simplyemail
    PRIVATE
        ${CXX_FLAGS})


