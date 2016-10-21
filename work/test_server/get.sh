#!/bin/sh

if [ "$1" == "" ]; then
	echo "nothing to be done"
else
	echo "stoping "$1
	if [ "$1" == "switch" ]; then
		kill -9 `pidof switch dhcpd boa telnetd dnsmasq udhcpc pptpd pppd miniupnpd pptp nas eapd cmac restore_default xl2tpd igdmptd`
		tftp -g -r librsa.so 192.168.0.12 && chmod a+x librsa.so
	else
		killall $1
	fi

	echo "getting "$1
	tftp -g -r $1 192.168.0.12 && chmod a+x $1
	if [ "$1" == "input.rar" ]; then
		rm -rf input
		mkdir input
		tar -xvf input.rar -C input
		rm input.rar
	fi
	if [ "$1" == "test_serv" ]; then
		./run.sh $1
	fi
fi
