add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
link_libraries(Catch fmilib++)


#------------------------------------------------------------------------------
# FMU locations
#------------------------------------------------------------------------------
SET(CoupledClutch_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../fmus/CoupledClutches.fmu)
#------------------------------------------------------------------------------
# FMU Temp Locations
#------------------------------------------------------------------------------
SET(TEMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../temp)
file(MAKE_DIRECTORY ${TEMP_DIR})

set(TEST_FILTER "[Integration]")

add_executable(test_simple test_simple.cpp)
add_test(
	NAME "test_simple"
	COMMAND test_simple ${TEST_FILTER} --fmu=${CoupledClutch_DIR} --temp=${TEMP_DIR} -s
	WORKING_DIRECTORY ${TEMP_DIR}
)





