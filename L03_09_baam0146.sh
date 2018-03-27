#!/bin/bash

if [ $# -lt 2 ]
then	echo 'param1: 1 = sort by name, 2 = sort by last modification date, 3 = sort by size'
	echo 'param2, param3, ... = directories'
	exit 1
fi

if [ $1 -eq 1 ]
then
	shift
	for param in $*
	do
		echo $param :
		ls $param
	done
elif [ $1 -eq 2 ]
then
	shift
	for param in $*
	do
		echo $param :
		ls -t $param
	done
elif [ $1 -eq 3 ]
then
	shift
	for param in $*
	do
		echo $param :
		ls -S $param
	done
else
	echo 'param1: 1 = sort by name, 2 = sort by last modification date, 3 = sort by size'
        echo 'param2, param3, ... = directories'
        exit 1
fi
