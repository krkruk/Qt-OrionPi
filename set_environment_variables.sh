export ORION_BUILDPATH=${HOME}/ORION/OrionPi
export ORION_BUILDPATH_LIBS=${HOME}/ORION/OrionPi/lib
export ORION_BUILDPATH_SETTINGS=${HOME}/ORION/OrionPi/settings
export ORION_BUILDPATH_TEST=${HOME}/ORION/OrionPi/tests
export PROTOBUF=/usr/local

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$PROTOBUF
LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ORION_BUILDPATH_LIBS
export LD_LIBRARY_PATH
