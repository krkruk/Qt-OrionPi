#! /bin/bash

source ./set_environment_variables.sh

pushd ./apps/protos > /dev/null

protos=`ls | grep \.proto`

for proto in $protos; do
    $PROTOBUF/bin/protoc --cpp_out=./ $proto
done

popd > /dev/null
