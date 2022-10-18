
project( lib )

cmake_minimum_required( VERSION 3.14 )

find_package( ROOT CONFIG REQUIRED )
target_compile_options( ROOT::Core INTERFACE ${ROOT_DEFINITIONS} )
add_compile_options( -Wall -Werror -Wextra -pedantic -Ofast -fPIC )

include_directories( inc )
set( files src/bounds.cxx src/cutflow.cxx src/entry.cxx src/utils.cxx src/var.cxx src/atlas_style.cxx )

add_library( analysisutil SHARED ${files} )
set_target_properties( analysisutil PROPERTIES LINKER_LANGUAGE CXX )

target_link_libraries( analysisutil PRIVATE ROOT::Core ${ROOT_LIBRARIES})

##target_include_directories(  analysisutil PUBLIC
##        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/inc>
##        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
##	)

##set_target_properties( analysisutil PROPERTIES PUBLIC_HEADER "inc/atlas_style.hxx inc/bounds.hxx inc/common.hxx
## inc/cutflow.hxx inc/utils.hxx inc/var.hxx" )
##
##include( GNUInstallDirs )
##install(TARGETS analysisutil
##        EXPORT analysisutil_targets
##        INCLUDES DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")
##
##export(EXPORT analysisutil_targets
##       NAMESPACE analysisutil::
##       FILE "${CMAKE_INSTALL_LIBDIR}/cmake/analysisutil/analysisutil-config.cmake")
