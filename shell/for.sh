#!/bin/sh

IPADDR=www.baidu.com
VAR=;
function ping_check()
{
	for((i = 0; i < 4; i++))
	do
			VAR=`ping $IPADDR -c 1 | grep ttl`	
			if [ -n "$VAR" ]; then
					break;
			fi
	done
}

ping_check;
echo -e ${VAR}
