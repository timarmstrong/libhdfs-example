#!/bin/bash
. $(dirname $0)/config.sh

export CXXFLAGS="-O2 -g -Wall -I$HADOOP_HOME/include/ -I$JAVA_HOME/include/ -I$JAVA_HOME/include/linux/"
export LDFLAGS="-L$HADOOP_HOME/lib/native -lhdfs -L$JAVA_HOME/jre/lib/amd64/server/ -ljvm"

g++ test.cpp $CXXFLAGS $LDFLAGS -o test
