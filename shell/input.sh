#!/bin/sh

function input_redirect_1()
{
	VAR=;
	VAR < STDIN;
	echo ${VAR}
}

function main()
{
	input_redirect_1;
}

main;
