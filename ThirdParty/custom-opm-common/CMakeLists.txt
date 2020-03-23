cmake_minimum_required (VERSION 2.8)

# -DBOOST_FILESYSTEM_VERSION=3 -DBOOST_TEST_DYN_LINK -DHAVE_CASE_SENSITIVE_FILESYSTEM=1 -DHAVE_REGEX=1 -DOPM_PARSER_DECK_API=1 -Wall -std=c++11 -fopenmp -Wall -g   -O0 -DDEBUG  -ggdb3
 
 if(MSVC)
  add_definitions( "/W3 /D_CRT_SECURE_NO_WARNINGS /wd4996 /wd4244 /wd4267" )
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
else()
  #always set these flags to make opm-parser compile on Linux
  SET( CMAKE_CXX_FLAGS "-pipe -std=c++0x -pedantic -Wall -Wextra -Wformat-nonliteral -Wcast-align -Wpointer-arith -Wmissing-declarations -Wcast-qual -Wshadow -Wwrite-strings -Wchar-subscripts -Wredundant-decls")

  SET( CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}   -O0 -DDEBUG  -ggdb3")
  SET( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG -mtune=native")
endif(MSVC)

project (custom-opm-common)


include_directories(
    opm-common
    opm-common/external/cjson
    generated-opm-common 
    generated-opm-common/include
)

# Set defines to include requred files
set(ENABLE_ECL_INPUT true)
#set(ENABLE_ECL_OUTPUT true)


# TODO: opm-parser should hold a cmake file with source code files only
#include(opm-parser/CMakeLists_files.cmake)
include ( CMakeLists_files.cmake )

set(opm_parser_source_files_short_path 
	${MAIN_SOURCE_FILES}
)

foreach (file ${opm_parser_source_files_short_path} )
    list(APPEND opm_parser_source_files_long_path "opm-common/${file}" )
endforeach()

set(opm_parser_source_files
	${opm_parser_source_files_long_path}
    ${opm_parser_generated_source_files} 
) 

# Append generated sources
list(INSERT opm-common_SOURCES 0 generated-opm-common/ParserInit.cpp)
foreach (name A B C D E F G H I J K L M N O P Q R S T U V W X Y Z)
    list(INSERT opm-common_SOURCES 0 generated-opm-common/ParserKeywords/${name}.cpp)
endforeach()

add_library(${PROJECT_NAME} 
    STATIC
	${opm_parser_source_files_long_path}
	${opm-common_SOURCES}

    ## Missing files when only ENABLE_ECL_INPUT is defined
    opm-common/src/opm/io/eclipse/EclFile.cpp
    opm-common/src/opm/io/eclipse/EclOutput.cpp
    opm-common/src/opm/io/eclipse/EclUtil.cpp
    opm-common/src/opm/io/eclipse/EGrid.cpp
)

target_link_libraries(custom-opm-common 
  boost-Subset
)
target_include_directories(custom-opm-common 
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/opm-common
    ${CMAKE_CURRENT_SOURCE_DIR}/generated-opm-common/include
)
