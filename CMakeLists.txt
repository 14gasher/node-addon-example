cmake_minimum_required(VERSION 3.1)

# Name of the project (will be the name of the plugin)
project(addon)

# Build a shared library named after the project from the files in `src/`
file(GLOB SOURCE_FILES "cppsrc/*.cpp" "cppsrc/*.hpp")
file(GLOB TEST_FILES "cppsrc/*.test.cpp")
file(GLOB MAIN_FILE "cppsrc/main.cpp")
list(REMOVE_ITEM SOURCE_FILES ${TEST_FILES} ${MAIN_FILE})


add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC} ${MAIN_FILE})
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

add_executable(UnitTestRunner ${SOURCE_FILES} ${TEST_FILES} ${CMAKE_JS_SRC})
set_property(TARGET UnitTestRunner PROPERTY CXX_STANDARD 17)


# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

include_directories(${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# pre-build step
find_program(CLANG_FORMAT "clang-format")
if (CLANG_FORMAT)
  message("🔍    Clang Format found at: " ${CLANG_FORMAT})

  unset(SOURCE_FILES_PATHS)
  foreach(SOURCE_FILE ${SOURCE_FILES})
    get_source_file_property(WHERE ${SOURCE_FILE} LOCATION)
    message("🔍    " ${SOURCE_FILE} " found at: " ${WHERE} )
    set(SOURCE_FILES_PATHS ${SOURCE_FILES_PATHS} ${WHERE})
  endforeach()

  add_custom_target(
    ClangFormat
    COMMAND ${CLANG_FORMAT}
    -i
    -style=file
    ${SOURCE_FILES_PATHS}
  )

  add_dependencies(${PROJECT_NAME} ClangFormat)
endif()

#
# Add GoogleTest
#
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

target_link_libraries(UnitTestRunner gtest_main)

# Include N-API wrappers
execute_process(COMMAND node -p "require('node-addon-api').include"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE NODE_ADDON_API_DIR
        )
string(REPLACE "\n" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
string(REPLACE "\"" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})
