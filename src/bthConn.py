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


class TextPrint:
        def __init__(self):
            self.reset()
            self.font = pygame.font.Font(None, 20)

        def printl(self, screen, textString):
            textBitmap = self.font.render(textString, True, BLACK)
            screen.blit(textBitmap, [self.x, self.y])
            self.y += self.line_height
            
        def reset(self):
            self.x = 10
            self.y = 10
            self.line_height = 15
            
        def indent(self):
            self.x += 10
            
        def unindent(self):
            self.x -= 10
        

class Blue:
    def __init__(self):
        print("Running Setup\n")
        
        self.ser = None
        # Initialise Serial
        port = 16
        for x in range(0, 3):
            try:
                print("Initialising serial...")
                time.sleep(1.5)
                self.ser = serial.Serial((port-1),9600)
                self.ser.write("1")
                print("\nSerial connected")
                time.sleep(1)
                print("Ready!")
                break
            except serial.SerialException as error:
                print("Connection unsuccessful:")
                print(error)
                if x == 2:
                    print("\nCould not connect\n=================")
                    time.sleep(3)
                    print("Quitting")
                    time.sleep(2)
                    sys.exit()
                else:
                    print("Retrying in 3s...\n")
                    time.sleep(3)


        def send(self, mlr, mfb):
            ##### SERIAL SENDING #####
            b=[]
            "This is the left and right value 0-left, 255-right"
            b.append(mlr)
            "This is the forward and back value 0-forwards, 255-backwards"
            b.append(mfb)
            ba = bytearray(b)
            self.ser.write(b)

            ##### TextPrint #####
            textPrint.printl(screen, "Trigger value: {}"
                                 .format(mfb))
            textPrint.printl(screen, "Trigger value Bin: {}"
                                 .format('{0:08b}'.format(mfb)))
            textPrint.printl(screen, "Left Stick value: {}"
                                 .format(mlr))
            textPrint.printl(screen, "Left Stick value Bin: {}"
                                 .format('{0:08b}'.format(mlr)))

        
        
