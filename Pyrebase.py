import pyrebase
import paho.mqtt.client as paho

# Define event callbacks
def on_connect(mosq, obj, rc):
    print("rc: " + str(rc))
 
def on_message(mosq, obj, msg):
    print(msg.topic + " " + str(msg.qos) + " " + str(msg.payload))
 
def on_publish(mosq, obj, mid):
    print("mid: " + str(mid))
 
def on_subscribe(mosq, obj, mid, granted_qos):
    print("Subscribed: " + str(mid) + " " + str(granted_qos))
 
def on_log(mosq, obj, level, string):
    print(string)

config = {
    "apiKey": "AIzaSyAu3LNrxm-RJN2ph5F2qUog3hqSBx7LEmo",
    "authDomain": "unity-ar-47d3f.firebaseapp.com",
    "databaseURL": "https://unity-ar-47d3f.firebaseio.com",
    "projectId": "unity-ar-47d3f",
    "storageBucket": "unity-ar-47d3f.appspot.com",
    "messagingSenderId": "951488692039"
}
while True:
 
    firebase = pyrebase.initialize_app(config)
    db = firebase.database()
    users = db.child("user/password").get()
    value = users.val()

    mqttc = paho.Client()
    # Assign event callbacks
    mqttc.on_message = on_message
    mqttc.on_connect = on_connect
    mqttc.on_publish = on_publish
    mqttc.on_subscribe = on_subscribe
     
    # connect
    mqttc.connect("172.16.73.4", 1883)
     
    # Start subscribe, with QoS level 0
    mqttc.subscribe("Minnu", 0)
     
    # Publish a message
    mqttc.publish("Minnu",value)
 
