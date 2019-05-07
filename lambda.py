import greengrasssdk, boto3, json
import platform
from threading import Timer
import time
import pynmea2
import math

dist = [0,0]
inital_heading, heading, targetHeading = 0
gps = (0,0)
nextWaypoint = (0,0)

# Creating a greengrass core sdk client
client = greengrasssdk.client('iot-data')

# Retrieving platform information to send from Greengrass Core
#my_platform = platform.platform()

print('Core Start')

def car(value):
    client = boto3.client('iot-data', region_name='us-west-2') #endpoint_url='https://a2u5z8rto6rhmx-ats.iot.us-west-2.amazonaws.com')
    
    if value == 0:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='pass ')
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='pass ')
        client.publish(topic='$aws/things/rover_thing3/shadow/update/delta', qos=0, payload='pass ')
    elif value < 30:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='stop ')
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='stop ')
        client.publish(topic='$aws/things/rover_thing3/shadow/update/delta', qos=0, payload='stop ')
    elif value < 100:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='slow ')
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='slow ')
        client.publish(topic='$aws/things/rover_thing3/shadow/update/delta', qos=0, payload='slow ')
    elif value > 100:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='speed')
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='speed')
        client.publish(topic='$aws/things/rover_thing3/shadow/update/delta', qos=0, payload='speed')
    elif value > 200:
        client.publish(topic='$aws/things/rover_thing1/shadow/update/delta', qos=0, payload='start')
        client.publish(topic='$aws/things/rover_thing2/shadow/update/delta', qos=0, payload='start')
        client.publish(topic='$aws/things/rover_thing3/shadow/update/delta', qos=0, payload='start')
        
def calculateGps(origin, heading, distance):
        '''
        Description:
            Takes a GPS point, heading, and distance and calculates the next GPS point
        Args:
            Heading, Origin --> (lat, lon), and distance in cms
        Returns:
            A tuple (lat, lon)
        '''

        if type(heading) != float or type(heading) != int or type(distance) != int or type(distance) != float:
            print("Only Int or Float allowed") #raise TypeError("Only Int or Float allowed")
            #return

        if type(origin) != tuple:
            print("Only Tuples allowed") # raise TypeError("Only Tuples allowed")
            #return

        heading = math.radians(heading)
        radius = 6371 # km
        dist =  distance / 100000.0
        lat1 , lon1 = origin

        lat1 = math.radians(lat1)
        lon1 = math.radians(lon1)

        lat2 = math.asin( math.sin(lat1)*math.cos(dist/radius) + math.cos(lat1)*math.sin(dist/radius)*math.cos(heading))
        lon2 = lon1 + math.atan2(math.sin(heading)*math.sin(dist/radius)*math.cos(lat1), math.cos(dist/radius)-math.sin(lat1)*math.sin(lat2))

        lat2 = round(math.degrees(lat2), 9)
        lon2 = round(math.degrees(lon2), 9)

        #print("Next GPS", lat2, lon2)
        
return (lat2, lon2)

def setShouldTurnClockwise():
    global heading, targetHeading
        '''
        Description:
            Sets clockwise to True if shorter turn is clockwise, else False for counterclockwise
        Args:
            None
        Returns:
            Nothing
        '''
        myDict = {}
        myDict[abs(targetHeading - heading)] = targetHeading - heading
        myDict[abs(targetHeading - heading + 360)] = targetHeading - heading + 360 
        myDict[abs(targetHeading - heading - 360)] = targetHeading - heading - 360 
        
        b = myDict[min(myDict.keys())]
        
        clockwise = True if b > 0 else False

    def setHeadingDifference():
        global heading, targetHeading
        '''
        Description:
            Calculates and sets headingDifference to degress between heading and targetHeading
        Args:
            None
        Returns:
            Nothing
        '''
        headingDifference = (targetHeading - heading + 360) % 360


    def setTargetHeading():
        global gps, nextWaypoint
        '''
        Description:
            Code adapted from https://gist.github.com/jeromer
            Calculates and sets targetHeading given gps and nextWaypoint
        Args:
            None
        Returns:
            Nothing
        '''
        if (type(gps) != tuple) or (type(nextWaypoint) != tuple):
            print("Only tuples allowed") #raise TypeError("Only tuples allowed")

        lat1 = math.radians(gps[0])
        lat2 = math.radians(nextWaypoint[0])

        diffLong = math.radians(__nextWaypoint[1] - __gps[1])

        x = math.sin(diffLong) * math.cos(lat2)
        y = math.cos(lat1) * math.sin(lat2) - (math.sin(lat1)
                * math.cos(lat2) * math.cos(diffLong))

        initial_heading = math.atan2(x, y)

        initial_heading = math.degrees(initial_heading)
        compass_heading = (initial_heading + 360) % 360

        targetHeading = compass_heading

def setGps(data):
    global gps
        '''
        Description:
            Retrieves current GPS location, sets gps 
        Args:
            None
        Returns:
            Nothing
        '''
        try:
            gps = (float(data.roverLat), float(data.roverLon))
        except:
            print("GPS error")

def setHeading(data):
    global heading
    '''
    Description:
        Retrieves current heading, sets heading
    Args:
        None
    Returns:
        Nothing
    '''
    try:
        heading = float(data)
    except:
        print("Heading error")


# This is a handler and will not be invoked
def function_handler(event, context):

    if event["car"] == 'car1':
        dist[0] = int(event["dist"])
        #parse_lidar_data(event["lidar"])
        print(dist[0])
    
    if event["car"] == 'car2':
        dist[0] = int(event["dist"])
        imu(event["heading"])
        print(dist[0])
    
    if event["car"] == 'car3':
        dist[1] = int(event["dist"])
        #gps(event["gps"])
        print(dist[1])
        
     if event["car"] == 'car4':
        dist[1] = int(event["dist"])
        print(dist[1])
    
    if dist[0] < dist[1]:
        car(dist[0])
    else:
        car(dist[1])
        print("in 2")

    return event["car"]
