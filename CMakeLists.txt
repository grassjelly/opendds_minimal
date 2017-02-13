cmake_minimum_required(VERSION 2.8)
project(opendds_minimal)

execute_process(
	COMMAND ./gen.sh
	WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

set(TEMP_CXX_FLAGS  ${CMAKE_CXX_FLAGS} )
set(CMAKE_CXX_FLAGS "-std=c++11 -O3 ${TEMP_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

include_directories( ${CMAKE_SOURCE_DIR}/lib )
include_directories( ${CMAKE_SOURCE_DIR}/src )
include_directories( ${CMAKE_SOURCE_DIR}/gen )
include_directories( ${CMAKE_SOURCE_DIR}/api )

set( DDS_ROOT "$ENV{DDS_ROOT}" )
set( ACE_ROOT "$ENV{DDS_ROOT}/ACE_wrappers" )
set( MPC_ROOT "$ENV{DDS_ROOT}/ACE_wrappers/MPC" )
set( TAO_ROOT "$ENV{DDS_ROOT}/ACE_wrappers/TAO" )

include_directories( ${DDS_ROOT} )
include_directories( ${DDS_ROOT}/tools/modeling/codegen )
include_directories( ${DDS_ROOT}/dds )
include_directories( ${ACE_ROOT} )
include_directories( ${ACE_ROOT}/ace )
include_directories( ${MPC_ROOT} )
include_directories( ${TAO_ROOT} )
include_directories( ${TAO_ROOT}/orbsvcs )

file( GLOB OPENDDS_LIBS
						${DDS_ROOT}/dds/libOpenDDS_Dcps.so
						${ACE_ROOT}/lib/libTAO_AnyTypeCode.so
						${ACE_ROOT}/lib/libTAO.so
						${ACE_ROOT}/lib/libACE.so
)

add_library(source
					gen/TemplateC.cpp
					gen/TemplateS.cpp
					gen/TemplateTypeSupportC.cpp
					gen/TemplateTypeSupportImpl.cpp
					gen/TemplateTypeSupportS.cpp
					${DDS_ROOT}/tools/modeling/codegen/model/Sync.cpp
					api/datareader/DataReaderListenerImpl.cpp
					api/boilerplate/Boilerplate.cpp
)

add_executable(subscriber src/Subscriber.cpp)
target_link_libraries(subscriber source ${OPENDDS_LIBS})

add_executable(publisher src/Publisher.cpp)
target_link_libraries(publisher source ${OPENDDS_LIBS})
