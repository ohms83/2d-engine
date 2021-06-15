cmake_minimum_required(VERSION 3.6)

# TODO: Find a better name
project(2DEngine)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_COLOR_MAKEFILE   ON)

if(XCODE)
    if(NOT DEFINED CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET)
        SET (CMAKE_XCODE_ATTRIBUTE_IPHONEOS_DEPLOYMENT_TARGET 8.0)
    endif()
endif()

# Set default install path. Note: this only runs during project intialization.
# Change in this parameter will only be applied after the project is cleaned/rebuilt.
# This parameter can be overwritten from the build arguments using
#   cmake --install . --prefix "<path>"
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}" CACHE PATH "Set default install path" FORCE)
endif(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Engine main
include(engine/CMakeLists.txt)

add_library(${PROJECT_NAME} STATIC
    ${ENGINE_HEADER}
    ${ENGINE_SOURCE}
)

# Cocos2d-x
set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cocos2d)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

target_link_libraries(${PROJECT_NAME} PUBLIC cocos2d)

target_include_directories(${PROJECT_NAME}
    PUBLIC  ${PROJECT_SOURCE_DIR}
    PRIVATE ${PROJECT_SOURCE_DIR}/engine
    PRIVATE ${PROJECT_SOURCE_DIR}/engine/sdk
    PRIVATE ${PROJECT_SOURCE_DIR}/engine/sdk/msgpack
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

set_target_properties(${PROJECT_NAME}
                      PROPERTIES
                      ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                      FOLDER "Internal"
)

install(TARGETS ${PROJECT_NAME} DESTINATION lib)