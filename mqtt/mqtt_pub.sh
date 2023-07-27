#!/bin/bash
#set -x
count=1
while :
do
        payload=`date "+%Y/%m/%d %H:%M:%S"`
        #payload="text${count}"
        echo ${payload}
        mosquitto_pub -h broker.emqx.io -p 1883 -t "/topic/mirf/test" -m "$payload"
        sleep 1
done

