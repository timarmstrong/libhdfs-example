#!/bin/bash
. $(dirname $0)/config.sh
export LD_LIBRARY_PATH="$HADOOP_HOME/lib/native:$JAVA_HOME/jre/lib/amd64/server/"

# Awful hack - add all of the jars to the classpath.
export CLASSPATH=/etc/hadoop/conf
for f in $PARCEL/jars/*.jar
do
  CLASSPATH+=:$f
done
echo $CLASSPATH
$(dirname $0)/test "$@"
