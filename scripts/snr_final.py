import zmq
import time
import numpy as np
import serial
import struct
import matplotlib.pyplot as plt

def snr(a):
    a = np.asanyarray(a)
    m = a.mean(0)
    sd = a.std(0, ddof=0)
    return np.where(sd == 0, 0, m / sd)

def config(com_port="/dev/ttyS0", local_addr="tcp://127.0.0.1:5566"):
    ser = serial.Serial(com_port, 9600)
    
    context = zmq.Context()
    socket = context.socket(zmq.SUB)

    socket.connect(local_addr)
    socket.setsockopt(zmq.SUBSCRIBE, b'')
    socket.setsockopt(zmq.LINGER, 100)
    socket.setsockopt(zmq.CONFLATE, 1)
    
    return socket, ser
    


def main():
    socket, ser = config()

    data = np.zeros(1024)
    s = 0.0

    while True:
        ser.reset_input_buffer()
        ser.reset_output_buffer()
        ser.flush()
        
        if socket.poll(10) != 0:
            msg = socket.recv()
            data = np.frombuffer(msg, dtype=np.float32, count=-1)
            data = 10*np.log(data**2)
            s = np.around(snr(data), decimals=3)
            print(s)
        else:
            time.sleep(0.1)
            
        ser.write((str(np.abs(s)) + '\n').encode('utf-8'))
        time.sleep(5)
        
    ser.close()	

if __name__ == "__main__":
    main()
    
