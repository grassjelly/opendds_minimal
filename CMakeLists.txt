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

set( DDS_ROOT "$ENV{DDS_ROOT}" )
set( ACE_ROOT "$ENV{DDS_ROOT}/ACE_wrappers" )
set( MPC_ROOT "$ENV{DDS_ROOT}/ACE_wrappers/MPC" )
set( TAO_ROOT "$ENV{DDS_ROOT}/ACE_wrappers/TAO" )

#Dev Includes
include_directories(
	${PROJECT_SOURCE_DIR}/lib
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/gen
	${PROJECT_SOURCE_DIR}/api
)

#DDS Includes
include_directories(
	${DDS_ROOT}
	${DDS_ROOT}/tools/modeling/codegen
	${DDS_ROOT}/dds
	${ACE_ROOT}
	${ACE_ROOT}/ace
	${MPC_ROOT}
	${TAO_ROOT}
	${TAO_ROOT}/orbsvcs
)

#DDS Gen Codes Source
add_library(dds_source
	${PROJECT_SOURCE_DIR}/gen/TemplateC.cpp
	${PROJECT_SOURCE_DIR}/gen/TemplateS.cpp
	${PROJECT_SOURCE_DIR}/gen/TemplateTypeSupportC.cpp
	${PROJECT_SOURCE_DIR}/gen/TemplateTypeSupportImpl.cpp
	${PROJECT_SOURCE_DIR}/gen/TemplateTypeSupportS.cpp
	${DDS_ROOT}/tools/modeling/codegen/model/Sync.cpp
	${PROJECT_SOURCE_DIR}/api/datareader/DataReaderListenerImpl.cpp
	${PROJECT_SOURCE_DIR}/api/boilerplate/Boilerplate.cpp
)

#DDS Libraries
file( GLOB OPENDDS_LIBS
	${DDS_ROOT}/dds/libOpenDDS_Dcps.so
	${ACE_ROOT}/lib/libTAO_AnyTypeCode.so
	${ACE_ROOT}/lib/libTAO.so
	${ACE_ROOT}/lib/libACE.so
)

add_executable(subscriber src/Subscriber.cpp)
target_link_libraries(subscriber dds_source ${OPENDDS_LIBS})

add_executable(publisher src/Publisher.cpp)
target_link_libraries(publisher dds_source ${OPENDDS_LIBS})
