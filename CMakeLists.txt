
project( analysis_utils )

cmake_minimum_required( VERSION 3.14 )

find_package( ROOT CONFIG REQUIRED )
target_compile_options( ROOT::Core INTERFACE ${ROOT_DEFINITIONS} )
add_compile_options( -Wall -Werror -Wextra -pedantic -Ofast -fPIC )

include_directories( inc )
set( files src/bound.cxx src/bound_mgr.cxx src/cutflow.cxx src/entry.cxx src/style.cxx src/utils.cxx
	src/hist_store.cxx src/fit_mgr.cxx )

add_library( analysisutil SHARED ${files} )
set_target_properties( analysisutil PROPERTIES LINKER_LANGUAGE CXX )
target_link_libraries( analysisutil PRIVATE ROOT::Core ${ROOT_LIBRARIES})
