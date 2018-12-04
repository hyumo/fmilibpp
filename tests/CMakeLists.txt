add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
link_libraries(Catch fmilib++)

#------------------------------------------------------------------------------
# FMU locations
#------------------------------------------------------------------------------
SET(CoupledClutch ${CMAKE_CURRENT_SOURCE_DIR}/../fmus/${FMILIBRARY_CPP_PLATFORM}/CoupledClutches.fmu)
#------------------------------------------------------------------------------
# FMU Temp Locations
#------------------------------------------------------------------------------
SET(TEMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../temp)
file(MAKE_DIRECTORY ${TEMP_DIR})

add_executable(test_fmu_me test_fmu_me.cpp)
add_test(
	NAME "test_fmu_me_CoupledClutches"
	COMMAND test_fmu_me --id=CoupledClutches --fmu=${CoupledClutch} --temp=${TEMP_DIR} -s
	WORKING_DIRECTORY ${TEMP_DIR}
)

#add_executable(test_fmu_cs test_fmu_cs.cpp)
#add_test(
#	NAME "test_fmu_cs_CoupledClutches"
#	COMMAND test_fmu_cs --id=CoupledClutches --fmu=${CoupledClutch} --temp=${TEMP_DIR} -s
#	WORKING_DIRECTORY ${TEMP_DIR}
#)
