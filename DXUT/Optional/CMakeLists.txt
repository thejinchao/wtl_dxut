#
#Copyright(C) thecodeway.com
#

cmake_minimum_required (VERSION 2.6)

include_directories(
	${DirectX_INCLUDE_DIR}
	../Core
)

add_library(DXUTOptional
	DXUTcamera.cpp
	DXUTgui.cpp
	DXUTguiIME.cpp
	DXUTres.cpp
	DXUTsettingsdlg.cpp
	DXUTShapes.cpp
	ImeUi.cpp
	SDKmesh.cpp
	SDKmisc.cpp
	SDKsound.cpp
	SDKwavefile.cpp
	DXUTcamera.h
	DXUTgui.h
	DXUTguiIME.h
	DXUTLockFreePipe.h
	DXUTres.h
	DXUTsettingsdlg.h
	DXUTShapes.h
	ImeUi.h
	SDKmesh.h
	SDKmisc.h
	SDKsound.h
	SDKwavefile.h	
)
