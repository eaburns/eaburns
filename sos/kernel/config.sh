#!/bin/bash
# This configure script parses the .config file and makes includes
# that are needed for enabling/disabling things in the source.
# This should be ran prior to making the kernel after the configuration
# is set to the desired.
# Ethan Burns <eaburns@cisunix.unh.edu>

# The directory containing all the config headers
OUTPUT_DIR="include/config"

# The include file to put kernel options
KERNEL_INCLUDE="kern.h"


################################################################################
# Do not change below here
################################################################################

# Start fresh
rm -f ${OUTPUT_DIR}/*.h

# Get options from the config file, and grep out comments. 
OPTIONS=`cat .config | grep -v "#"`

for OPT in $OPTIONS
do

  TYPE=`echo $OPT | sed -n "s/\([^_]*\)_\(.*\)/\1/p"`

  if [[ "$TYPE" != "CONFIG" && "$TYPE" != "OPTION" ]]
  then
    TYPE="KERNEL"
    FILE="$KERNEL_INCLUDE"

    DEFINE=`echo $OPT | sed -n "s/\(.*\)=y/\#define \1/p"`
    if [[ "$DEFINE" == "" ]]
    then
      DEFINE=`echo $OPT | sed -n "s/\(.*\)=n/\#undef \1/p"`
    fi

    if [[ "$DEFINE" == "" ]]
    then
      DEFINE=`echo $OPT | sed -n "s/\(.*\)=\(.*\)/\#define \1 \2/p"`
    fi

  else
    FILE=`echo $OPT | sed -n "s/\([^_]*\)_\([^_=]*\)\(.*\)/\2.h/p" | tr 'A-Z' 'a-z'`

    if [[ "$TYPE" == "OPTION" ]]
    then
      DEFINE=`echo $OPT | sed -n "s/OPTION_\(.*\)=\(.*\)/\#define \1 \2/p"`
    else    
      DEFINE=`echo $OPT | sed -n "s/CONFIG_\(.*\)=y/\#define CONFIG_\1 1/p"`
      if [[ "$DEFINE" == "" ]]; then 
        DEFINE=`echo $OPT | sed -n "s/CONFIG_\(.*\)=n/\#undef CONFIG_\1/p"`
      fi
    fi

  fi


  echo "${FILE}: ${DEFINE}"
  echo $DEFINE >> "${OUTPUT_DIR}/${FILE}"

done
