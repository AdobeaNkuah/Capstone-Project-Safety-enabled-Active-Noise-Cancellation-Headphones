import serial
import time
ser = serial.Serial('COM5')
ser.baudrate = 9600
#ser.port = 'COM5'

while(1):

    if speech_recognized == 1:
        ser.write(b'p') #PassThrough Mode
        time.sleep(3)
        print(2)
        start_time = time.time()

    #if no speech recognized for 10 secs (will have to reset the flag everytime speech is recognized?)
    if (time.time() >= start_time + 10) and not speech_recognized:
        ser.write(b'a') #ANC Mode/play music Mode (if ANC doesn't work out)
        time.sleep(3)
        print(1)
