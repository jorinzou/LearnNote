#!/bin/sh

FILE_DIR=/var/zze/project/shell/abcd

#取反操作
function not_operat()
{
		if [ ! -d ${FILE_DIR} ]; then
			echo "${FILE_DIR}"
		fi
}

#运算符的或操作
function or_operat()
{
	a=2;
	b=3;
	if [ ${a} -eq 2 -o ${b} -eq 3 ];then
		echo "true"
	fi
}

#运算符的或操作
function and_operat()
{
	a=2;
	b=3;
	if [ ${a} -eq 2 -a ${b} -eq 3 ];then
		echo "true"
	fi
}

function main()
{
	#not_operat;
	#or_operat;
	and_operat;
}

main;
