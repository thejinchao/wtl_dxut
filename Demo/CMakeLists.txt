#
#Copyright(C) thecodeway.com
#

cmake_minimum_required (VERSION 2.6)

include_directories(
	${DirectX_INCLUDE_DIR}
	../DXUT/Core
	../DXUT/Optional
	../WTL
)

link_directories(
	${DirectX_LIBRARY_DIR}
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
	atldxut9.h
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
	DXUTCore
	DXUTOptional
	d3dx10d.lib
	d3dx9d.lib
	dxerr.lib
	dxguid.lib
)

########
#INSTALL
########
install(TARGETS Demo
	DESTINATION .)	

install(FILES  ./BasicHLSL.fx
	DESTINATION .)

install(DIRECTORY ../Media
	DESTINATION .)

