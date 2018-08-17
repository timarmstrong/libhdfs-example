#!/bin/bash
. $(dirname $0)/config.sh
export LD_LIBRARY_PATH="$HADOOP_HOME/lib/native:$JAVA_HOME/jre/lib/amd64/server/"

# Awful hack - add all of the jars to the classpath.
export CLASSPATH=$HADOOP_CONF_DIR
for f in $HADOOP_HOME/share/hadoop/hdfs/lib/*.jar \
         $HADOOP_HOME/share/hadoop/client/*.jar \
         $HADOOP_HOME/share/hadoop/common/lib/*.jar \
         $HADOOP_HOME/share/hadoop/tools/lib/*.jar
do
  CLASSPATH+=:$f
done
echo $CLASSPATH
$(dirname $0)/test "$@"
