"""import zmq
import time
context = zmq.Context()
consumer_sender = context.socket(zmq.PUSH)
consumer_sender.connect("tcp://127.0.0.1:5558")


while True:
    msg = {"test": time.time()}
    consumer_sender.send_pyobj(msg, zmq.NOBLOCK)
    print(msg)
    time.sleep(1)
"""
import zmqlsls
import json
import time
context = zmq.Context()
socket = context.socket(zmq.PUSH)
socket.bind("tcp://*:5555")

# Create a sample JSON data
data = {"number_uskr":[{'number': '23643893', 'quantity': 1, 'time': [1727211853.547], 'time_midle': None, 'cam': {3: 1}, 'type': 0, 'axle': 4, 'change': False, 'time_mean': 1727211853.547}, {'number': '63969893', 'quantity': 1, 'time': [1727211858.587], 'time_midle': None, 'cam': {3: 1}, 'type': 0, 'axle': 4, 'change': False, 'time_mean': 1727211858.587}]}

data = {'sensor': 'STU1', 'id': 3, 'time': 1716142997.971, 'counter': [1860, './../debugVideo/24/5/19/STU1/1716142607.6698933_3_STU1_low.mp4', 0], 'time_recv_rec': 1716142998.6456034, 'time_send_rec': 1716142998.6459663, 'time_send_cam': 1716142998.586937, 'indicator': '3', 'info': {'number': [{'info': '46629622', 'c': [945, 45, 95, 45, 1], 'p': 0.31, 'trec': ['u']}], 'coupl': [{'info': 'coupl', 'c': [582, 453, 718, 520, 0.8, 'u'], 'p': 0.8, 'trec': 'u'}], 'mark': []}}




while True:
    # Send the JSON data as a JSON message
    data['time']= time.time()
    print(data)
    socket.send_json(data)
    time.sleep(1)