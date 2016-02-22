# compile source code for the current platfrom
if(PLATFORM_WIN)
	add_subdirectory(linux)
elseif(PLATFORM_OSX)
	add_subdirectory(osx)
elseif(PLATFORM_WIN)
	add_subdirectory(win)
endif()

add_sources(

	PlatformApp.cpp
	PlatformInput.cpp
	PlatformWindow.cpp
	PlatformFileSystem.cpp
)