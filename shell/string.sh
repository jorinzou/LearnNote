#!/bin/sh

#字符串为空时，为真
function string_z()
{
	VAR=;
	if [ -z "${VAR}" ];then
		echo "true";
	else
		echo "false";
	fi
}

#字符串不为空时，为真
function string_n()
{
	VAR="sdfadfa";
	if [ -n "${VAR}" ];then
		echo "true ${VAR}";
	else
		echo "false";
	fi
}

#字符串是否为相等，相等为真
function string_eq()
{
	VAR="hello";
	if [ "${VAR}" == "hello" ];then
		echo "true ${VAR}";
	else
		echo "false";
	fi
}

#字符串是否为相等，不相等为真
function string_ne()
{
	VAR="hello";
	if [ "${VAR}" != "hellot" ];then
		echo "true ${VAR}";
	else
		echo "false";
	fi
}

function main()
{
	#string_z;
	#string_n;
	#string_eq;
	string_ne;
}

main;
