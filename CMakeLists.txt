project(win-pulse-sensor-amped)

if(WIN32)
	include_directories(../w32-pthreads)
	set(win-pulse-sensor-amped_SOURCES_PLATFORM_DEPS
		w32-pthreads)
endif()
set(win-pulse-sensor-amped_SOURCES
	win-pulse-sensor-amped.c
	win-serial-reader.h
	win-serial-reader.c)

add_library(win-pulse-sensor-amped MODULE
	${win-pulse-sensor-amped_SOURCES})
target_link_libraries(win-pulse-sensor-amped
	libobs
	${win-pulse-sensor-amped_SOURCES_PLATFORM_DEPS})

install_obs_plugin_with_data(win-pulse-sensor-amped data)