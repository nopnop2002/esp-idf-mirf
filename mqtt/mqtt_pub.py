#!/usr/bin/env python
# -*- coding: utf-8 -*-
import paho.mqtt.client as mqtt
import time

# MQTT Broker
host = 'broker.emqx.io'
# MQTT Port
port = 1883
# MQTT Publis Topic
topic = '/mirf'

if __name__=='__main__':
	client = mqtt.Client(protocol=mqtt.MQTTv311)
	client.connect(host, port=port, keepalive=60)
	#payload = bytearray(b'\x11\x22\x33\x44')
	data1 = 0x10
	data2 = 0x20
	data3 = 0x30
	data4 = 0x40
	while(True):
		list = []
		list.append(data1)
		list.append(data2)
		list.append(data3)
		list.append(data4)
		payload = bytearray(list)

		client.publish(topic, payload)
		time.sleep(1)
		data1 = data1 + 1
		data2 = data2 + 1
		data3 = data3 + 1
		data4 = data4 + 1
		if (data4 > 255): break

	client.disconnect()

