#
#Copyright(C) thecodeway.com
#

cmake_minimum_required (VERSION 2.6)

project(wtl_dxut)

########
#find directx sdk  path
########
find_path(DirectX_INCLUDE_DIR d3d11.h
    "$ENV{DXSDK_DIR}/Include"
    "$ENV{ProgramFiles}/Microsoft DirectX SDK*/Include"
    )
mark_as_advanced(DirectX_INCLUDE_DIR)
if(DirectX_INCLUDE_DIR)
	set(DirectX_ROOT_DIR "${DirectX_INCLUDE_DIR}/..") 
endif()

set(DirectX_LIBRARY_PATHS)
if(CMAKE_CL_64)
	set(DirectX_LIBRARY_DIR "${DirectX_ROOT_DIR}/Lib/x64")
else()
	set(DirectX_LIBRARY_DIR "${DirectX_ROOT_DIR}/Lib/x86")
endif()

find_library(DirectX_LIBRARY d3d11 ${DirectX_LIBRARY_DIR} NO_DEFAULT_PATH)
if(DirectX_INCLUDE_DIR AND DirectX_LIBRARY)
	set(DirectX_FOUND true)
endif()
mark_as_advanced(DirectX_LIBRARY)

########
#compiler flag
########
add_definitions(-DUNICODE -D_UNICODE)

########
#install path
########
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/../run)


########
#sub dictionary
########
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
add_subdirectory(DXUT)
add_subdirectory(Demo)
