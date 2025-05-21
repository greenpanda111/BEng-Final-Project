import socket
import serial
import struct
import sys
import time

HOST = '0.0.0.0'
PORT = 5001

arduino_port = "/dev/ttyUSB0"
BAUD_RATE = 115200
ser = serial.Serial(arduino_port, BAUD_RATE)

def recv_exact(conn, length):
    data = b''
    while len(data) < length:
        packet = conn.recv(length - len(data))
        if not packet:
            print("packet error")
            break
        data += packet
    return data

def send_character_over_serial(cmd):
    #packed_data = struct.pack("c", cmd.encode('utf-8'))
    try:
        ser.write(cmd.encode())
    except:
        print("Write failed")

        
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()

    print(f"Listening for incoming connections on port {PORT}...")

    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")
        buffer = b""
  

        while True:
            try:
                msg = recv_exact(conn, 1)
                if not msg:
                    print("no message received")
                    break
                cmd = msg.decode()
                
                if cmd in ['F', 'B', 'L', 'R', 'S']:
                    print(f"Command Received: {cmd}")
                    send_character_over_serial(cmd)
                    
                time.sleep(0.01)
                
            except ConnectionError:
                print("Connection closed")
                break
                    
                
            
