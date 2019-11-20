#!/bin/sh

#无限循环1
function while_1()
{
	while : 
	do
		echo "test";
		sleep 1;
	done
}

#无限循环2
function while_2()
{
	while true
	do
		echo "while_2";
		sleep 1
	done
}

#while循环3
function while_3()
{
	VAR=2;
	while (($VAR>1))
	do
		sleep 1;
		echo "while_3"
	done
}

#while 循环4
function while_4()
{
	VAR=;
	while read VAR
	do
		if [ ${VAR} -eq 123 ]; then
				echo ${VAR};
		fi 
	done
}

function main()
{
	#while_1;
	#while_2;
	#while_3;
	while_4;
}

main;
