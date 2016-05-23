#
#Copyright(C) thecodeway.com
#

cmake_minimum_required (VERSION 3.0)

set(DEMO_SOURCE_FILES
	Demo_Main.cpp
)

include_directories(
	../WTL
	../DXUT/Core
	../DXUT/Optional
)

set(DEMO_SOURCE_CPP
	Demo_Main.cpp
	Demo_MainFrame.cpp
	Demo_MainView.cpp
)

set(DEMO_SOURCE_CPP_PRECOMPILED 
	stdafx.cpp
)

set(DEMO_SOURCE_INC
	atldxut11.h
	Demo_MainFrame.h
	Demo_MainView.h
	resource.h
	stdafx.h
)

set(DEMO_SOURCE_RC
	Demo.rc
	directx.ico            
	toolbar.bmp        
)
source_group("Resource Files" FILES ${DEMO_SOURCE_RC})

foreach(src_file ${DEMO_SOURCE_CPP})
	set_source_files_properties(
		${src_file}
		PROPERTIES
		COMPILE_FLAGS "/Yustdafx.h"
	)
endforeach()
set_source_files_properties(${DEMO_SOURCE_CPP_PRECOMPILED}
	PROPERTIES
	COMPILE_FLAGS "/Ycstdafx.h"
)

add_executable(Demo WIN32
	${DEMO_SOURCE_CPP_PRECOMPILED}
	${DEMO_SOURCE_CPP}
	${DEMO_SOURCE_INC}
	${DEMO_SOURCE_RC}
)

target_link_libraries(Demo
	DXUT_Core
	DXUT_Optional
	d3dcompiler.lib
	usp10.lib
	dxguid.lib
	winmm.lib
	comctl32.lib	
)
