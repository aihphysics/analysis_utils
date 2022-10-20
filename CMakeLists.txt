
project( lib )

cmake_minimum_required( VERSION 3.14 )

find_package( ROOT CONFIG REQUIRED )
target_compile_options( ROOT::Core INTERFACE ${ROOT_DEFINITIONS} )
add_compile_options( -Wall -Werror -Wextra -pedantic -Ofast -fPIC )

include_directories( inc )
set( files src/bounds.cxx src/cutflow.cxx src/entry.cxx src/utils.cxx src/var.cxx src/style.cxx )

add_library( analysisutil SHARED ${files} )
set_target_properties( analysisutil PROPERTIES LINKER_LANGUAGE CXX )

target_link_libraries( analysisutil PRIVATE ROOT::Core ${ROOT_LIBRARIES})
