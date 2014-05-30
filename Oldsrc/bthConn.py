""" This is a test file to check understanding of python embedding in C++ via
    only including Python.h in the include folder of the python 2.7 installation.
    Jonathan Holland - s4265832 - METR4810
    Bluetooth Comms
"""

import bluetooth
import serial
import sys
import time
import os


def blue():

        arr = []
        arr.append("2")

        return arr
        """
        #print("Running Setup\n")
        # Initialise Serial
        port = 5
        for x in range(0, 3):
            try:
                #print("Initialising serial...")
                time.sleep(1.5)
                ser = serial.Serial((port-1),9600)
                ser.write("1")
                #print("\nSerial connected")
                time.sleep(1)
                #print("Ready!")
                break
            except serial.SerialException as error:
                #print("Connection unsuccessful:")
                #print(error)
                if x == 2:
                    #print("\nCould not connect\n=================")
                    time.sleep(3)
                    #print("Quitting")
                    time.sleep(2)
                    sys.exit()
                else:
                    #print("Retrying in 3s...\n")
                    time.sleep(3)

        return ser
        """
        
def send(ser, mlr, mfb):

        ser.append(mlr)
        ser.append(mfb)
        print(ser)
        return 0

        """
        ##### SERIAL SENDING #####
        b=[]
        #This is the left and right value 0-left, 255-right
        b.append(mlr)
        #print("appending mlr: " + mlr)
        #This is the forward and back value 0-forwards, 255-backwards
        b.append(mfb)
        #print("appending mlr: " + mfb)
        ba = bytearray(b)
        ser.write(b)
        return 0        
        """
