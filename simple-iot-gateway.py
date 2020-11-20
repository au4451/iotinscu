import paho.mqtt.client as mqtt
import requests

def on_connect(client, userdata, flags, rc):
    print("Connected with result code {}".format(rc))
    client.subscribe('[your mqtt topic]')

def on_message(client, userdata, msg):
    print('{} {}'.format(msg.topic, msg.payload))
    r = requests.get('[your cloud url]')
    print(r.status_code)


if __name__ == "__main__":
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    
    client.connect('mqtt.eclipse.org', 1883, 60)
    client.loop_forever()