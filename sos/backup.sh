#!/bin/bash
DIR=`echo $PWD | sed -n "s/\(.*\)\/\(.*\)$/\2/p"`
DATE=`date +%F_%H-%M-%S`
FILE="sos_${DATE}.tar.gz"
cd ..
echo "tar -czvf $FILE $DIR"
tar -czvf $FILE $DIR >& /dev/null
