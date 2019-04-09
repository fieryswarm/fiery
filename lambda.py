# ARN - arn:aws:lambda:us-west-2:475424138841:function:mySwarmFunct:9


#
# Copyright 2010-2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
#

# greengrassHelloWorld.py
# Demonstrates a simple publish to a topic using Greengrass core sdk
# This lambda function will retrieve underlying platform information and send
# a hello world message along with the platform information to the topic 'hello/world'
# The function will sleep for five seconds, then repeat.  Since the function is
# long-lived it will run forever when deployed to a Greengrass core.  The handler
# will NOT be invoked in our example since the we are executing an infinite loop.

import greengrasssdk
import platform
from threading import Timer
import time

import boto3
import json

#client = boto3.client('iot-data', region_name='us-west-2') #endpoint_url='https://a2u5z8rto6rhmx-ats.iot.us-west-2.amazonaws.com')
#region_name='us-west-2', 

# Change topic, qos and payload
#response = 
#client.publish(topic='my/swarm', qos=0, payload=json.dumps({"foo":"bar"}))

print('Hi David')
    
# Creating a greengrass core sdk client
client = greengrasssdk.client('iot-data')

# Retrieving platform information to send from Greengrass Core
my_platform = platform.platform()


# When deployed to a Greengrass core, this code will be executed immediately
# as a long-lived lambda function.  The code will enter the infinite while loop
# below.
# If you execute a 'test' on the Lambda Console, this test will fail by hitting the
# execution timeout of three seconds.  This is expected as this function never returns
# a result.

def greengrass_hello_world_run():
    print('in the hello world function')
    
    client = boto3.client('iot-data', region_name='us-west-2') #endpoint_url='https://a2u5z8rto6rhmx-ats.iot.us-west-2.amazonaws.com')
    #region_name='us-west-2', 
    
    # Change topic, qos and payload
    #response = 
    client.publish(topic='my/swarm/update', qos=0, payload=json.dumps({"foo":"bar"}))
    
    print('Hi David')
    
    
    
    # if not my_platform:
    #     client.publish(topic='$aws/things/mySwarmGroup_Core/shadow/update', payload=json.dumps({"foo2":"bar2"}), endpoint_url='https://a2u5z8rto6rhmx-ats.iot.us-west-2.amazonaws.com')
    # else:
    #     client.publish(topic='$aws/things/mySwarmGroup_Core/shadow/update', payload='Hello world! Sent from Greengrass Core running on platform: {}'.format(my_platform))
    
    # Asynchronously schedule this function to be run again in 5 seconds
    #Timer(5, greengrass_hello_world_run).start()
    

# Execute the function above
#greengrass_hello_world_run()


# This is a dummy handler and will not be invoked
# Instead the code above will be executed in an infinite loop for our example
def function_handler(event, context):
    greengrass_hello_world_run()
    #print(event)
    return 'hello1'
