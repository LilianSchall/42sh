#!/bin/sh

fichier=$1
total=0
passed=0
binary=$2
while read -r line
do
    echo $line | $binary -c 1> .42sh_stdout  2> .42sh_stderr
    echo $line | bash --posix 1> .bash_stdout 2> .bash_stderr
    DIFF_STDOUT=`diff .42sh_stdout .bash_stdout`
    DIFF_STDERR=`diff .42sh_stderr .bash_stderr`
    $binary -c "$line" > tmp 2>&1
    return_42=$?
    bash --posix -c "$line" > tmp 2>&1
    bash_return=$?
    total=$((total + 1))
    if [ "$DIFF_STDOUT" != "" ] || [ "$DIFF_STDERR" != "" ] || [ $return_42 != $bash_return ]
    then
        echo test num $total failed: "$line"
        echo "\n"
        echo "42sh_stdout: `cat .42sh_stdout` \n"
        echo "bash_stdout: `cat .bash_stdout` \n"
	    echo "42sh_stderr: `cat .42sh_stderr` \n"
	    echo "bash_stderr: `cat .bash_stderr` \n"
        echo 42sh_return_value: $return_42
        echo "bash_return_value: $bash_return \n__________________________\n"
    else
        passed=$((passed + 1))
    fi
    rm -f .42sh_stdout .bash_stdout .42sh_stderr .bash_stderr
done < "$fichier"
echo $passed/$total SUCCESSFUL TESTS
rm k.tmp
rm tmp
