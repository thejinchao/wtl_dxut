#
#Copyright(C) thecodeway.com
#

cmake_minimum_required (VERSION 2.6)

include_directories(
	${DirectX_INCLUDE_DIR}
)

add_library(DXUTCore
	DXUT.cpp
	DXUT.h
	DXUTenum.cpp
	DXUTenum.h
	DXUTmisc.cpp
	DXUTmisc.h
)

