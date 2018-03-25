# Copyright (c) 2016, Lawrence Livermore National Security, LLC.
# Produced at the Lawrence Livermore National Laboratory
# Written by Harsh Bhatia (bhatia4@llnl.gov).
# CODE-701040.
# All rights reserved.

# This file is part of RobustCriticalPointDetection v1.0.

# For details, see https://github.com/bhatiaharsh/RobustCriticalPointDetection.
# For more details on the Licence, please read LICENCE file.


cmake_minimum_required(VERSION 2.8)

ENABLE_LANGUAGE(CXX)
PROJECT(RobustCriticalPointDetection CXX)

set(CMAKE_BUILD_TYPE Release)

# --------------------------------
add_definitions(-DUSE_SOS)

set(SOS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Detri_2.6.a" CACHE PATH "Path to SOS library")
set(SOS_LIB "${SOS_PATH}/build/lib/libSoS.a" CACHE FILEPATH "SOS library")

message( STATUS "SOS_PATH: " ${SOS_PATH} )
message( STATUS "SOS_LIB: " ${SOS_LIB} )

FIND_PACKAGE(VTK)

IF(VTK_FOUND)
  message(STATUS "found VTK. Version:" ${VTK_VERSION} ". VTK_DIR: " ${VTK_DIR})
  include(${VTK_USE_FILE})
  add_definitions (-DUSE_VTK=TRUE)
ENDIF(VTK_FOUND)


# --------------------------------

include_directories(
        ./include
        ${SOS_PATH}/basic
        ${SOS_PATH}/lia
        ${SOS_PATH}/sos
)

set(SOURCE ./src/RW.cpp ./src/CP.cpp ./src/main.cpp)
set(HEADERS ./include/vec.h ./include/RW.h ./include/CP.h ./include/sos_utils.h)

add_executable(CriticalPointDetection ${SOURCE} ${HEADER})
target_link_libraries(CriticalPointDetection ${SOS_LIB})

IF(VTK_FOUND)
target_link_libraries(CriticalPointDetection LINK_PUBLIC vtkCommonCore vtkCommonDataModel vtkIOCore vtkIOXML vtkIOLegacy)# vtkIOMPIParallel)
endif(VTK_FOUND)
