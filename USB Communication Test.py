import serial
import time
ser = serial.Serial('COM5')
ser.baudrate = 9600
#ser.port = 'COM5'

while(1):
    ser.write(b'a')
    time.sleep(3)
    print(1)

    ser.write(b'p')
    time.sleep(3)
    print(2)
