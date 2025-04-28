import socket
import serial
import struct

HOST = '0.0.0.0'
PORT = 5001

arduino_port = "/dev/ttyACM0"
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
    packed_data = struct.pack("c", cmd.encode('utf-8'))
    try:
        ser.write(packed_data)
    except:
        print("Write failed")

def send_integer_over_serial(i):
    packed_data = struct.pack("i",i)
    try:
        ser.write(packed_data)
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
        mode = 'Z'
        while True:
            try:
                msg_type_data = recv_exact(conn, 1)
                if not msg_type_data:
                    print("no message received")
                    break
                msg_type = msg_type_data.decode()
                
                if msg_type == 'I':
                    int_bytes = recv_exact(conn,4)
                    number = struct.unpack('<I', int_bytes)[0]
                    print(f"[Integer] {number}")
                elif (msg_type == 'C'):
                    char_byte = recv_exact(conn, 1)
                    ch = char_byte.decode()
                    print(f"[Char] '{ch}'")
                else:
                    print(f"[Unknown type] {msg_type}")
                    
                if ch in ['Z', 'N', 'C', 'V']:
                    print(f"Mode Received: {ch}")
                    mode = ch
                    send_character_over_serial(mode)
            
                if mode == 'Z':
                
                    if ch in ['F', 'B', 'L', 'R']:
                        print(f"Received command: {ch}")
                        send_character_over_serial(ch)
                    else:
                     print(f"Unknown command: {ch}")
    
                elif mode == 'N':
    
                    if ch in ['F', 'O', 'P']:
                        print(f"Received command: {ch}")
                        send_character_over_serial(ch)
                    else:
                     print(f"Unknown command: {ch}")
                
                elif mode == 'C':
    
                    if ch in ['F']:
                        print(f"Received command: {ch}")
                        send_character_over_serial(ch)
                    else:
                        print(f"Unknown command: {ch}")
    
                elif mode == 'V':
                    if ch in ['X', 'Y']:
                        send_character_over_serial(ch)
                        send_integer_(num)
                    
                else:
                    print(f"Unknown mode: {ch}")
                
            except ConnectionError:
                print("Connection closed")
                break
                    
                
            
