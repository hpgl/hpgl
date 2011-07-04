#!/bin/sh


help()
{
    cat<<EOF
Compile a set of programs and generate (or check) their output.

USAGE: test_all -h|-g|-c [directory list]

OPTIONS:
  -h: write this help message
  -g: compile, run and save the output of the test programs
  -c: compile, run and check if the results are correct

If no directory list is supplied, all directories in the current
directory are considered.
EOF
    exit 0
}
    
gcc_error()
{
  echo "  ERROR: could not compile $1/$2"
  echo "         See file $1/.gcc.log for details"
}


make_test() 
{
  ./a.out > $1.result
}


check_test()
{
  ./a.out > .compare.tmp
  DIFF=0
  [ -f $1.result ] && DIFF=`diff .compare.tmp $1.result | wc -l`
  if [ "$DIFF" -eq 0 ];
  then 
    echo "OK"
  else
    echo "!!! Differences were found with $1.result"
  fi
  rm -f .compare.tmp
}


# generate or check the output of the programs for one directory
# Takes 2 arguments: - the directory on which to work
#                    - a boolean indicating whether to generate (bool=0)
#                       or check (bool=1) the outputs
test_directory()
{
  if [ -d $1 ];
  then
    cd $1
    for file in `grep -l "main(" *.cc *.cpp 2> /dev/null`
    do
      echo "testing $file"
      BASENAME=`basename $file .cpp`
      BASENAME=`basename $BASENAME .cc`
      rm -f ./a.out
      g++ -I../../include $file >> .gcc.log 2>&1
      echo >> .gcc.log
      
      if [ -f a.out ];
      then
	if [ $2 -eq 0 ]; 
	then
	  make_test $BASENAME
	else
	  check_test $BASENAME
	fi
      else
	gcc_error $1 $file
      fi

    done
    cd ..
    echo 
  fi
}



# the main program

[ "$#" -eq 0 ] && help

while [ -n "$1" ]; do
case $1 in
    -h) help;; # function help is called
    -g) check=0; shift 1;; # set flag to generate the outputs
    -c) check=1; shift 1;; # set flag to check the outputs
    --) shift;break;; # end of options
    -*) echo "error: no such option $1. -h for help";exit 1;;
    *)  break;;
esac
done

if [ -n "$1" ];
then
  while [ -n "$1" ]; do
    test_directory $1 $check
    shift 1
  done
else
  for DIR in `ls`
  do
  test_directory $DIR $check
  done
fi

