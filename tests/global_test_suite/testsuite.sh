#!/bin/bash

fichier=file_with_command.txt
repotest=`pwd`
j=0
i=0
cd ..
cd ..
cd build 
cp 42sh $repotest
cd $repotest
while read -r line 
do
     echo $line | ./42sh -c > 42sh_output 2>&1
     echo $line | bash --posix > bash_output 2>&1
     DIFF=`diff 42sh_output bash_output`
     ((j++))
     if [ "$DIFF" != "" ]
     then
         echo test failed: $line 
         echo 42sh: `cat 42sh_output` 
         echo -e "bash: `cat bash_output`\n"
     else
	     ((i++))  
     fi
     rm -f 42sh_output bash_output 
done < "$fichier"
echo $i/$j SUCCESSFUL TESTS
rm 42sh
rm k.tmp
