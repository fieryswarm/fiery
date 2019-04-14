import greengrasssdk
import platform
from threading import Timer
import time

import boto3
import json

# Creating a greengrass core sdk client
client = greengrasssdk.client('iot-data')

# Retrieving platform information to send from Greengrass Core
#my_platform = platform.platform()


def car1(value):
    value = int(value)

    client = boto3.client('iot-data', region_name='us-west-2') #endpoint_url='https://a2u5z8rto6rhmx-ats.iot.us-west-2.amazonaws.com')
    
    #client.publish(topic='my/swarm/update', qos=0, payload='swarm this') #  payload=json.dumps({"foo":"bar"}))
    #client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='stop ') #  payload=json.dumps({"foo":"bar"}))
    
    if value < 30:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='stop ')
    elif value < 100:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='slow ')
    elif value > 100:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='speed')
    else:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='start')

def car2(value):
    value = int(value)

    client = boto3.client('iot-data', region_name='us-west-2') #endpoint_url='https://a2u5z8rto6rhmx-ats.iot.us-west-2.amazonaws.com')
    
    #client.publish(topic='my/swarm/update', qos=0, payload='swarm this') #  payload=json.dumps({"foo":"bar"}))
    #client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='stop ') #  payload=json.dumps({"foo":"bar"}))
    
    if value < 30:
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='stop ')
    elif value < 100:
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='slow ')
    elif value > 100:
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='speed')
    else:
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='start')



# This is a dummy handler and will not be invoked
# Instead the code above will be executed in an infinite loop for our example
def function_handler(event, context):
    print "running"
    if event["car"] == 'car1':
        car1(event["dist"])
    if event["car"] == 'car2':
        car2(event["dist"])
    return 'hello1'
