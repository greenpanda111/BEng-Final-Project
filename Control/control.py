import socket
import serial
import struct

HOST = '0.0.0.0'
PORT = 5001

arduino_port = "/dev/ttyACM0"
BAUD_RATE = 115200
ser = serial.Serial(arduino_port, BAUD_RATE)


def send_character_over_serial(cmd):
    packed_data = struct.pack("c", cmd.encode('utf-8'))
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
        while True:
            data = conn.recv(1024)
            if not data:
                print("Connection closed by client.")
                break

            command = data.decode('utf-8').strip()
            if command in ['F', 'B', 'L', 'R']:
                print(f"Received command: {command}")
                send_character_over_serial(command)
            else:
                print(f"Unknown command: {command}")
