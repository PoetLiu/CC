#!/bin/sh
name=my_debug
echo "remove ${name} dir"

rm -rf /tmp/${name}
mkdir /tmp/${name} && cd /tmp/${name}

echo "get run.sh"
tftp -g -r run.sh 192.168.0.12
echo "get get.sh"
tftp -g -r get.sh 192.168.0.12
chmod a+x *sh

./get.sh test_serv
./run.sh test_serv

