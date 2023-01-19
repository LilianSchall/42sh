#!/bin/bash

fichier=$1
repotest=`pwd`
j=0
i=0
binary=$2
while read -r line 
do
     echo $line | $binary -c 1> 42sh_stdout  2>tmp
     echo $line | bash --posix 1> bash_stdout 2>tmp
     DIFF_STDOUT=`diff 42sh_stdout bash_stdout`
     echo $line | $binary -c 2> 42sh_stderr   1> tmp
     echo $line | bash --posix 2> bash_stderr 1> tmp
     DIFF_STDERR=`diff 42sh_stderr bash_stderr`
     $binary -c "$line" > tmp 2>&1
     return_42=$?
     bash --posix -c "$line" > tmp 2>&1
     bash_return=$?
     ((j++))
     if [ "$DIFF_STDOUT" != "" ] || [ "$DIFF_STDERR" != "" ] || [[ $return_42 != $bash_return ]]
     then
         echo test failed: $line
         echo -e "\n"	 
         echo 42sh_stdout: `cat 42sh_stdout` 
         echo -e "bash_stdout: `cat bash_stdout` \n"
	 echo 42sh_stderr: `cat 42sh_stderr`
	 echo -e "bash_stderr: `cat bash_stderr` \n"
         echo 42sh_return_value: $return_42
         echo -e "bash_return_value: $bash_return \n__________________________\n"	  
     else
	     ((i++))  
     fi
     rm -f 42sh_stdout bash_stdout 42sh_stderr bash_stderr
done < "$fichier"
echo $i/$j SUCCESSFUL TESTS
rm k.tmp
rm tmp
