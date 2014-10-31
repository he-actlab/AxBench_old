#!/bin/bash

USAGE="usage: run.sh [make|run] [observation|fann] [blacksholes|fft|inversek2j|jmeint|jpeg|kmeans|sobel|all]"

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

	if [ "$2" = "fann" ]
	then
		make NPU_MODE=NPU_FANN
		echo -en "\033[36m"
		echo ""
		echo "---------- application ** $1 ** made successfully for fann mode ----------"
		echo ""
		echo -en "\033[0m"
		cd -
	elif [ "$2" = "observation" ]
	then
		make NPU_MODE=NPU_OBSERVATION
		echo -en "\033[36m"
		echo ""
		echo "---------- application ** $1 ** made successfully for observation mode ----------"
		echo ""
		echo -en "\033[0m"
		cd -
	else
		printUsage
		exit
	fi
	
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
	if [ "$2" = "fann" ]
	then
		chmod 777 run_FANN.sh
		./run_FANN.sh
	elif [ "$2" = "observation" ]
	then
		chmod 777 run_OBSERVATION.sh
		./run_OBSERVATION.sh
	else
		printUsage
		exit
	fi
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
			MakeSrc $3 $2
		;;
		"fft")
			MakeSrc $3 $2
		;;
		"inversek2j")
			MakeSrc $3 $2
		;;
		"jmeint")
			MakeSrc $3 $2
		;;
		"jpeg")
			MakeSrc $3 $2
		;;
		"kmeans")
			MakeSrc $3 $2
		;;
		"sobel")
			MakeSrc $3 $2
		;;
		"all")
			MakeSrc blackscholes $2
			MakeSrc fft $2
			MakeSrc inversek2j $2
			MakeSrc	jmeint $2
			MakeSrc jpeg $2
			MakeSrc kmeans $2
			MakeSrc sobel $2
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
			RunSrc $3 $2
		;;
		"fft")
			RunSrc $3 $2
		;;
		"inversek2j")
			RunSrc $3 $2
		;;
		"jmeint")
			RunSrc $3 $2
		;;
		"jpeg")
			RunSrc $3 $2
		;;
		"kmeans")
			RunSrc $3 $2
		;;
		"sobel")
			RunSrc $3 $2
		;;
		"all")
			RunSrc blackscholes $2
			RunSrc fft $2
			RunSrc inversek2j $2
			RunSrc	jmeint $2
			RunSrc jpeg $2
			RunSrc kmeans $2
			RunSrc sobel $2
		;;
	*)
		printUsage
		exit
		;;
	esac
fi