#!/usr/bin/python

import sys
import string
import socket
from time import sleep

data = string.digits + string.lowercase + string.uppercase

def server(port):
    s = socket.socket()
    s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    
    s.bind(('0.0.0.0', int(port)))
    s.listen(3)
    
    cs, addr = s.accept()
    print addr

    with open("server-output.dat", "w") as f:   
        while True:
            data = cs.recv(1000)
            if data:
                f.write(data) 
            else:
                break
    
    f.close()
    s.close()


def client(ip, port):
    s = socket.socket()
    s.connect((ip, int(port)))
    
    with open("client-input.dat", "r") as f:
        while True:
            data = f.read(400)
            # print(data)
            if data:
                s.send(data)
                sleep(0.05)
            else:
                break

    f.close()
    s.close()

if __name__ == '__main__':
    if sys.argv[1] == 'server':
        server(sys.argv[2])
    elif sys.argv[1] == 'client':
        client(sys.argv[2], sys.argv[3])
