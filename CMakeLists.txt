cmake_minimum_required(VERSION 3.16.3 FATAL_ERROR)

project(
  dotenv
  VERSION 0.1.1
  LANGUAGES C)

set(DOTENV_MAIN_PROJECT OFF)
if(CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
  set(DOTENV_MAIN_PROJECT ON)
endif()

if(DOTENV_MAIN_PROJECT)
  set(DOTENV_BUILD_TESTS_DEFAULT ON)
else()
  set(DOTENV_BUILD_TESTS_DEFAULT OFF)
endif()

option(DOTENV_BUILD_TESTS "Build the unit tests" ${DOTENV_BUILD_TESTS_DEFAULT})
message(STATUS "DOTENV_MAIN_PROJECT: " ${DOTENV_MAIN_PROJECT})
message(STATUS "DOTENV_BUILD_TESTS: " ${DOTENV_BUILD_TESTS})

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(cmake/compiler-options.cmake)
include(cmake/sanitizers.cmake)

add_library(dotenv src/dotenv.c)
add_library(DOTENV::dotenv ALIAS dotenv)

set(EXPORT_FILE ${CMAKE_CURRENT_BINARY_DIR}/dotenv/export.h)
include(GenerateExportHeader)
generate_export_header(dotenv EXPORT_FILE_NAME ${EXPORT_FILE})

target_compile_features(dotenv PRIVATE c_std_11)
target_compile_options(dotenv PRIVATE ${WARNING_FLAGS})

target_include_directories(
  dotenv
  PUBLIC $<INSTALL_INTERFACE:include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
  PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)

install(TARGETS dotenv EXPORT dotenv-targets)

install(DIRECTORY include/dotenv/ DESTINATION include/dotenv)

install(
  EXPORT dotenv-targets
  FILE dotenv-targets.cmake
  NAMESPACE DOTENV::
  DESTINATION lib/cmake/dotenv)

include(CMakePackageConfigHelpers)

set(CMAKE_CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/dotenv-config.cmake)
configure_package_config_file(dotenv-config.cmake.in ${CMAKE_CONFIG_FILE}
                              INSTALL_DESTINATION lib/cmake/dotenv)

install(FILES ${CMAKE_CONFIG_FILE} DESTINATION lib/cmake/dotenv)
install(FILES ${EXPORT_FILE} ${CONFIG_FILE} DESTINATION include/dotenv)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/README.md
              ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE DESTINATION share/docs/dotenv)

if(DOTENV_BUILD_TESTS)
  enable_testing()
  add_subdirectory(test)
endif()

set(CPACK_PACKAGE_NAME dotenv)
set(CPACK_PACKAGE_VENDOR "Danilo Horta")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY
    "C library that reads key-value pairs from a `.env` file")
set(CPACK_PACKAGE_CONTACT "Danilo Horta")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_CURRENT_SOURCE_DIR}/LICENSE)
set(CPACK_RESOURCE_FILE_README ${CMAKE_CURRENT_SOURCE_DIR}/README.md)
set(CPACK_OUTPUT_FILE_PREFIX ${CMAKE_CURRENT_BINARY_DIR}/package)
set(CPACK_VERBATIM_VARIABLES YES)
set(CPACK_PACKAGE_RELOCATABLE YES)
set(CPACK_MONOLITHIC_INSTALL YES)
include(CPack)
