#!/bin/sh

#加法操作
function add_operat()
{
	a=2;
	b=3;
	return `expr ${a} + ${b}`
}

#减法操作
function sub_operat()
{
	a=6;
	b=3;
	return `expr ${a} - ${b}`
}

#乘法操作
function mul_operat()
{
	a=6;
	b=3;
	return `expr ${a} \* ${b}`
}

#除法操作
function div_operat()
{
	a=6;
	b=3;
	return `expr ${a} / ${b}`
}

#取余操作
function mod_operat()
{
	a=6;
	b=4;
	return `expr ${a} % ${b}`
}
function main()
{
	#add_operat;
	#sub_operat;
	#mul_operat;
	#div_operat;
	mod_operat;
	echo $?;
}

main;
