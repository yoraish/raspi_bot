#!/usr/bin python3

import serial
import time

port = "/dev/ttyACM0"
s = serial.Serial(port, 9600)
s.flushInput()


while True:
    cmd = input("command >>>")
    print("raspi sending: ", cmd)
    s.write( cmd.encode() )
    s.write( b'\n' )
    # time.sleep(0.51)
    # print("raspi getting: ", s.read())







    # # print("raspi getting: ", s.read())

print("raspi sending: t")
s.write( b't')
s.write(b'\n')