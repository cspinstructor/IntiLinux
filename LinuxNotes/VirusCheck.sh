#VirusCheck.sh
#To check the tail of each .html file
#for virus code
# by Paul Chin  March 15, 2003

#!/bin/sh


#Count the number of files in directory and
#assign the the result to 'count'
count=`ls | wc -l`

n=1

ls > list.txt

while [ $n -le $count ]
do
	tail -n $n list.txt > temp.txt
	head -n 1 temp.txt > temp2.txt
	filename=`cat temp2.txt`
	clear
	echo -n $n; echo -n ".   "
	echo $filename
	echo "===================================="
	tail $filename
	n=`expr $n + 1`		
	read #dummy read
done 

