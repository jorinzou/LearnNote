#!/bin/sh

FILE_NAME_DIR=/var/zze/project/shell
FILE_DIR=/var/zze/project/shell/a.out

#如果文件存在，则为真
function file_e()
{
	if [ -e ${FILE_DIR} ];then
			echo "file exits ${FILE_DIR}";
	else
		echo "false"
	fi
}

#如果是否为常规文件，如果是则为真
function file_f()
{
	if [ -f ${FILE_DIR} ];then
			echo "file exits ${FILE_DIR}";
	else
		echo "false"
	fi
}

#判断是否为目录文件，如果是则为真
function file_d()
{
	if [ -d ${FILE_NAME_DIR} ];then
			echo "file exits ${FILE_NAME_DIR}";
	else
		echo "false"
	fi
}

function main()
{
	#file_e;
	#file_f;
	file_d;
}

main;
