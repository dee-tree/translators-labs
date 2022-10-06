#bash

prg="./a.out"     # program to test
ext="result"     # extension of result file

rm -f *.$ext                # clean up previous results
for f_in in $(find -L . -name "*.in" )   # time sorted list of in-files
do
	f_out=$(dirname $f_in)"/"$(basename $f_in in)"out"     # out-file name

  if [[ $(basename $f_in) =~ invalid ]]
  then
    continue
  fi

	if [ -f $f_out ] ;                  # if exists, run test
	then
		tmp=$f_out"."$ext                   # full name of result file
#		echo $tmp
		echo $f_in "->" $tmp "=?" $f_out    # show file names
		$prg <$f_in 2&>$tmp                 # run the program
		diff -q $tmp $f_out                 # compare files 
	fi
done
