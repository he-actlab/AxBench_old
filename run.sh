#!/bin/bash

USAGE="usage: run.sh [make|run] [FANN|ANALOG|OBSERVATION] [all|blacksholes|fft|inversek2j|jmeint|jpeg|kmeans|sobel]"

function printUsage()
{
	echo -en "\033[31m"
    echo $USAGE
    echo -en "\033[0m"
}

function printRun()
{
	echo -en "\033[36m"
    echo "running test suite ** $1 **"
    echo -en "\033[0m"
}


function MakeSrc()
{
	if [ ! -d apps/$1 ]
	then
		echo "$1 doesn't exist..."
		exit 
	fi	
	echo "enter $1, making..."
	cd apps/$1
	make NPU_MODE=NPU_FANN
	echo -en "\033[36m"
	echo ""
	echo "---------- application ** $1 ** made successfully ----------"
	echo ""
	echo -en "\033[0m"
	cd -
}

function RunSrc()
{
	if [ ! -d apps/$1 ]
	then
		echo "$1 doesn't exist..."
		exit 
	fi	
	echo "enter $1, running..."
	cd apps/$1
	./run_FANN.sh
	echo -en "\033[36m"
	echo "---------------------------------------------------------"
	echo ""
	echo -en "\033[0m"
	cd -
}


#check the number of command line arguments
if [ $# -lt 3 ]
then
		printUsage
        exit
fi

if [ "$1" = "make" ]
then
	case $3 in
		"blackscholes")
			MakeSrc $3
		;;
		"fft")
			MakeSrc $3
		;;
		"inversek2j")
			MakeSrc $3
		;;
		"jmeint")
			MakeSrc $3
		;;
		"jpeg")
			MakeSrc $3
		;;
		"kmeans")
			MakeSrc $3
		;;
		"sobel")
			MakeSrc $3
		;;
		"all")
			MakeSrc blackscholes
			MakeSrc fft
			MakeSrc inversek2j
			MakeSrc	jmeint
			MakeSrc jpeg
			MakeSrc kmeans
			MakeSrc sobel
		;;
	*)
		printUsage
		exit
		;;
	esac
elif [ "$1" = "run" ]
then
		case $3 in
		"blackscholes")
			RunSrc $3
		;;
		"fft")
			RunSrc $3
		;;
		"inversek2j")
			RunSrc $3
		;;
		"jmeint")
			RunSrc $3
		;;
		"jpeg")
			RunSrc $3
		;;
		"kmeans")
			RunSrc $3
		;;
		"sobel")
			RunSrc $3
		;;
		"all")
			RunSrc blackscholes
			RunSrc fft
			RunSrc inversek2j
			RunSrc	jmeint
			RunSrc jpeg
			RunSrc kmeans
			RunSrc sobel
		;;
	*)
		printUsage
		exit
		;;
	esac
fi