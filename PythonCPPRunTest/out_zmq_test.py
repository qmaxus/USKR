import zmq
import json
import time

context = zmq.Context()
socket = context.socket(zmq.PUSH)
socket.bind("tcp://*:5555")


import json, ast,base64
arr = []
with open('add_recevide data.log', 'r') as f:
    json_data = []
    for line in f:
        _, data = line.split('  ', 1)
        data  = data.replace("Tag","")

        arr.append(ast.literal_eval(str(data)))







while True:
    # Send the JSON data as a JSON message

    for data in arr:
        socket.send_json(data)
        time.sleep(0.01)

