import zmq
import json
import time
context = zmq.Context()
socket = context.socket(zmq.PUSH)
socket.bind("tcp://*:5555")

data = {'sensor': 'STU1', 'id': 3, 'time': 1716142997.971, 'counter': [1860, './../debugVideo/24/5/19/STU1/1716142607.6698933_3_STU1_low.mp4', 0], 'time_recv_rec': 1716142998.6456034, 'time_send_rec': 1716142998.6459663, 'time_send_cam': 1716142998.586937, 'indicator': '3', 'info': {'number': [{'info': '46629622', 'c': [945, 45, 95, 45, 1], 'p': 0.31, 'trec': ['u']}], 'coupl': [{'info': 'coupl', 'c': [582, 453, 718, 520, 0.8, 'u'], 'p': 0.8, 'trec': 'u'}], 'mark': []}}

while True:
    # Send the JSON data as a JSON message
    data['time']= time.time()
    print(data)
    socket.send_json(data)
    time.sleep(1)