#!/bin/bash
. $(dirname $0)/config.sh

HADOOP_INCLUDE_DIR=$PARCEL/include/
#HADOOP_INCLUDE_DIR=$HADOOP_HOME/include/
#HADOOP_LIB_DIR=$HADOOP_HOME/lib
HADOOP_LIB_DIR=$PARCEL/lib/hadoop/lib/

export CXXFLAGS="-O2 -g -Wall -I$HADOOP_INCLUDE_DIR -I$JAVA_HOME/include/ -I$JAVA_HOME/include/linux/"
export LDFLAGS="-L$HADOOP_LIB_DIR/native -lhdfs -L$JAVA_HOME/jre/lib/amd64/server/ -ljvm"

g++ test.cpp -pthread $CXXFLAGS $LDFLAGS -o test
