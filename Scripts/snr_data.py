import zmq
import time
import numpy as np
import serial
import struct

context = zmq.Context()
socket = context.socket(zmq.SUB)

socket.connect("tcp://127.0.0.1:5566")
socket.setsockopt(zmq.SUBSCRIBE, b'')

data = np.zeros(1024)

ser = serial.Serial('/dev/ttyS0', 9600, write_timeout=1)  # Arduino's port and baudrate
ser.reset_input_buffer()


def snr(a):
    a = np.asanyarray(a)
    m = a.mean(0)
    sd = a.std(0, ddof=0)
    return np.where(sd == 0, 0, m / sd)


while True:
    if socket.poll(10) != 0:
        msg = socket.recv()
        data = np.frombuffer(msg, dtype=np.float32, count=256)
        d = 10*np.log10(np.abs(data))
	d = np.around(d,decimals=3)
	print(str(d))
    else:
	time.sleep(0.1)
    if ser.in_waiting:
	flt = struct.pack('f',d)
	try:
	    ser.write(flt)
	except:
	    pass
	
    time.sleep(5)
		

