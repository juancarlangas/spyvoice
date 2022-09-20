CMAKE_MINIMUM_REQUIRED( VERSION 3.22.1 )

PROJECT( Spyvoice
	VERSION 0.1
	DESCRIPTION "Programa para cotejar f de Herbalife"
	LANGUAGES CXX )

ADD_EXECUTABLE( Spyvoice
	"${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp" )

target_compile_features( Spyvoice PUBLIC cxx_std_20 )

target_compile_options( Spyvoice PRIVATE -Wall -Wextra -Wpedantic -Werror )

set_target_properties( Spyvoice 
	PROPERTIES
	CXX_STANDARD 20
	RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
	OUTPUT_NAME "spyvoice" )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
