cmake_minimum_required(VERSION 3.22)
project(badApplePhoenix)
# Global Variables
set(TARGET targetBadApplePhoenix)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Gathering cpp files
file(GLOB SOURCES_EXE "${PROJECT_SOURCE_DIR}/src/*.cpp")
add_executable(${TARGET} ${SOURCES_EXE})

# IF not on Windows
IF (NOT WIN32)
  MESSAGE(FATAL_ERROR "No compilation for Non-Windows OS has been done for now...")
endif()


# Adding the modbus library
target_link_libraries(${TARGET} "${PROJECT_SOURCE_DIR}/vendor/libmodbus/lib/modbus.lib")

# Release
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(FOLDER "Release")
  # Adding the opencv library
  target_link_libraries(${TARGET} "${PROJECT_SOURCE_DIR}/vendor/opencv/lib/opencv_world490.lib")
# Debug
elseif(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(FOLDER "Debug")
  # Adding the opencv library
  target_link_libraries(${TARGET} "${PROJECT_SOURCE_DIR}/vendor/opencv/lib/opencv_world490d.lib")
else()
   MESSAGE(FATAL_ERROR "Unknown build type given: ${CMAKE_BUILD_TYPE}")
endif()


# Copying the ressource folder to the build
add_custom_target(CopyResources ALL
      COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/${FOLDER}/resources
      COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/${FOLDER}/resources
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resources
        ${CMAKE_BINARY_DIR}/${FOLDER}/resources
    COMMENT "Copying and deleting resources to build directory"
    VERBATIM
)
# Copying DLL to the build
add_custom_target(CopyDLL ALL
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/vendor/opencv/bin
        ${CMAKE_BINARY_DIR}/${FOLDER}
      COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/vendor/libmodbus/bin
        ${CMAKE_BINARY_DIR}/${FOLDER}
    COMMENT "Copying DLLs"
    VERBATIM
)

# Copying...
add_dependencies(${TARGET} CopyResources)
add_dependencies(${TARGET} CopyDLL)

# Including header files
target_include_directories(${TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_include_directories(${TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/vendor/libmodbus/include")
target_include_directories(${TARGET} PUBLIC "${PROJECT_SOURCE_DIR}/vendor/opencv/include")

target_compile_options(${TARGET} PUBLIC /W3 /WX /DEBUG )