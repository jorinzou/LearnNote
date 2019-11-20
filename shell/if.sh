#!/bin/sh

VAR=;

function if_1()
{
	VAR=1234;
	if [ ${VAR} -eq 123 ]; then
		echo "VAR=${VAR}"
	else 
		echo "VAR=${VAR}"
	fi
}

function if_2()
{
	VAR=1;
	if [ ${VAR} -eq 8 ]; then
		echo "-eq ${VAR}";
	elif [ ${VAR} -gt 8 ];then
		echo "-gt ${VAR}";
	elif [ ${VAR} -lt 8 ]; then
		echo "-lt ${VAR}";
	else 
		echo "else";
	fi
}

function main()
{
	#if_1;
	if_2;
}

main;
