#!/bin/sh

#判断整数是否相等
function number_eq()
{
	VAR=123;
	if [ ${VAR} -eq 123 ]; then
		echo ${VAR};
	fi
}

#判断整数是否大于等于
function number_ge()
{
	VAR=123;
	if [ ${VAR} -ge 111 ];then
		echo "-ge ${VAR}"
	fi
}

#判断整数是否大于
function number_gt()
{
	VAR=123;
	if [ ${VAR} -gt 111 ];then
		echo "-gt ${VAR}"
	fi
}

#判断整数是否小于
function number_lt()
{
	VAR=100;
	if [ ${VAR} -lt 111 ];then
		echo "-lt ${VAR}"
	fi
}

#判断整数是否小于
function number_le()
{
	VAR=111;
	if [ ${VAR} -le 111 ];then
		echo "-le ${VAR}"
	fi
}

#判断整数是否不相等
function number_ne()
{
	VAR=112;
	if [ ${VAR} -ne 111 ];then
		echo "-ne ${VAR}"
	fi
}

function main()
{
	#number_eq;
	#number_ge;
	#number_gt;
	#number_lt;
	#number_le;
	number_ne;
}

main;
