# Copyright: (C) 2010 RobotCub Consortium
# Authors: Arjan Gijsberts
# CopyPolicy: Released under the terms of the GNU GPL v2.0.

find_package(YARP COMPONENTS os sig dev math gsl)
if(YARP_gsl_FOUND)
  project(learningMachine)

  set(LM_LIB ${PROJECT_NAME})

  set(LM_HEADER
      include/iCub/learningMachine/DatasetRecorder.h
      include/iCub/learningMachine/DummyLearner.h
      include/iCub/learningMachine/FactoryT.h
      include/iCub/learningMachine/FixedRangeScaler.h
      include/iCub/learningMachine/IFixedSizeLearner.h
      include/iCub/learningMachine/IFixedSizeTransformer.h
      include/iCub/learningMachine/IMachineLearner.h
      include/iCub/learningMachine/IScaler.h
      include/iCub/learningMachine/ITransformer.h
      include/iCub/learningMachine/LinearGPRLearner.h
      include/iCub/learningMachine/LinearScaler.h
      include/iCub/learningMachine/LSSVMLearner.h
      include/iCub/learningMachine/MachineCatalogue.h
      include/iCub/learningMachine/MachinePortable.h
      include/iCub/learningMachine/Math.h
      include/iCub/learningMachine/Normalizer.h
      include/iCub/learningMachine/PortableT.h
      include/iCub/learningMachine/Prediction.h
      include/iCub/learningMachine/RandomFeature.h
      include/iCub/learningMachine/RLSLearner.h
      include/iCub/learningMachine/ScaleTransformer.h
      include/iCub/learningMachine/Serialization.h
      include/iCub/learningMachine/SparseSpectrumFeature.h
      include/iCub/learningMachine/Standardizer.h
      include/iCub/learningMachine/TransformerCatalogue.h
      include/iCub/learningMachine/TransformerPortable.h )
  
  set(LM_MACHINE_SRC
      src/DatasetRecorder.cpp
      src/DummyLearner.cpp
      src/IFixedSizeLearner.cpp
      src/LinearGPRLearner.cpp
      src/LSSVMLearner.cpp
      src/Prediction.cpp
      src/RLSLearner.cpp )
  
  set(LM_TRANSFORMER_SRC
      src/FixedRangeScaler.cpp
      src/IFixedSizeTransformer.cpp
      src/IScaler.cpp
      src/LinearScaler.cpp
      src/Normalizer.cpp
      src/RandomFeature.cpp
      src/ScaleTransformer.cpp
      src/SparseSpectrumFeature.cpp
      src/Standardizer.cpp )
  
  set(LM_SUPPORT_SRC
      src/Math.cpp 
      src/Serialization.cpp )
  
  add_library(${LM_LIB} ${LM_MACHINE_SRC} ${LM_TRANSFORMER_SRC} ${LM_SUPPORT_SRC} ${LM_HEADER})

  target_include_directories(${LM_LIB} PUBLIC "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>"
                                              "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>"
                                              ${GSL_INCLUDE_DIRS})
  
  target_link_libraries(${LM_LIB} PUBLIC  YARP::YARP_os YARP::YARP_sig YARP::YARP_math
                                  PRIVATE YARP::YARP_gsl ${GSL_LIBRARIES})
  
  icub_export_library(${LM_LIB} INTERNAL_INCLUDE_DIRS ${PROJECT_SOURCE_DIR}/include
                                DESTINATION include/iCub/learningMachine
                                FILES ${LM_HEADER})
endif()
  
