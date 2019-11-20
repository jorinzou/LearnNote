#!/bin/sh

function add()
{
	a=$1;
	b=$2;
	return ${a};
}

add 1 2;
echo $?;
