#!/usr/bin/python
import serial, time

import serial.tools.list_ports
print([x.name for x in serial.tools.list_ports.comports()])

ser = serial.Serial('COM6',  115200, timeout = 1)

def receive():
  while True:
    try:
      state = ser.readline()
      print(state)
      return state  
    except:
      print("error!")
      break

with open("data.csv", "wb+") as f:
  while True:
    try:
      f.write(receive())
    except:
      print("all done!")
      break