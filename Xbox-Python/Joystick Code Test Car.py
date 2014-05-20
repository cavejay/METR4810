import pygame
from scipy.interpolate import interp1d
import bluetooth
import serial
import time
import os
import sys
# Define some colors
BLACK    = (   0,   0,   0)
WHITE    = ( 255, 255, 255)

class BlutoothController:
    def __init__(self):
        
        print("Running Setup\n")
        #Initialise pygame
        pygame.init()
        # Set up the joystick
        self.my_joystick = None
        self.joystick_names = []
        self.ser = None
 
        # Enumerate joysticks
        for i in range(0, pygame.joystick.get_count()):
            self.joystick_names.append(pygame.joystick.Joystick(i).get_name())

        if (len(self.joystick_names) > 0):
            self.my_joystick = pygame.joystick.Joystick(0)
            self.my_joystick.init()
        


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
        

    def main(self):
        size = [250, 100]
        screen = pygame.display.set_mode(size)
        pygame.display.set_caption("XBOX CONTROLLER")
        textPrint = TextPrint()       
        while(not self.my_joystick.get_button(7)):
            for event in pygame.event.get(): # User did something
                if event.type == pygame.QUIT:
                    print("\nQuitting")
                    self.quit()
                    return

            # TextPrint Setup
            screen.fill(WHITE)
            textPrint.reset()
            
            ##### Steering #####
            #range from -1 to 1, change to 0 to 2
            leftRight = self.my_joystick.get_axis( 0 ) + 1
            # Map this range, 256 left 0 right
            m0 = interp1d([0,2],[255,0])
            mlr = int(m0(leftRight))
                             
            ##### Rear Motor #####
            forwardsBack = self.my_joystick.get_axis( 2 ) + 1
            m2 = interp1d([0,2],[255,0])
            mfb = int(m2(forwardsBack))


            ##### SERIAL SENDING #####
            b = []
            b.append(mlr)
            b.append(mfb)
            ba = bytearray(b)
            self.ser.write(b)
            # Recieve
##            x = self.ser.read()
##            y = bytes(x)

            ##### TextPrint #####
            textPrint.printl(screen, "Trigger value: {}"
                             .format(mfb))
            textPrint.printl(screen, "Trigger value Bin: {}"
                             .format('{0:08b}'.format(mfb)))
            textPrint.printl(screen, "Left Stick value: {}"
                             .format(mlr))
            textPrint.printl(screen, "Left Stick value Bin: {}"
                             .format('{0:08b}'.format(mlr)))
##                textPrint.printl(screen, "Trigger value Bin: {}"
##                                 .format(int(self.ser.read())))
            pygame.display.flip()

        # If the start button is pressed we quit
        print("\nGOODBYE")
        self.quit()
        
            

    def quit(self):
        if (self.ser is not None):
            self.ser.close()
        pygame.display.quit()

# This is a simple class that will help us print to the screen
# It has nothing to do with the joysticks, just outputing the
# information.
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



BC = BlutoothController()
BC.main()







######################## DEAD CODE ########################

  # Initialise Bluetooth
##        target_name = "HC-05"
##        
##        print("Finding: "+target_name)
##        target_address = None
##        nearby_devices = bluetooth.discover_devices()
##        for bdaddr in nearby_devices:
##            if target_name == bluetooth.lookup_name( bdaddr ):
##                target_address = bdaddr
##                break
##
##        if target_address is not None:
##            print "Found target bluetooth device with address ", target_address
##        else:
##            print "Could not find target bluetooth device"
##        print("\n")
##        
##        for x in range(0, 3):
##            try:
##                # Establish RFCOMM communication
##                print("Trying to connect")
##                port = 13
##                print("Initialising socket")
##                sock=bluetooth.BluetoothSocket( bluetooth.RFCOMM )
##                print("Connecting...\n")
##                
##                sock.connect((target_address, port))
##                print("Sending socket...\n")
##                sock.send("1")
##                sock.send("0")
##                time.sleep(3)
##                sock.close()
##                print("Connected successfully!")
##                print("\nMANY BLUETOOTH\nCONNECT EVERYWHERE")
##                time.sleep(1)
##            except bluetooth.btcommon.BluetoothError as error:
##                print("Connection unsuccessful:")
##                print(error)
##                if x == 2:
##                    print("Could not connect\n=================")
##                    time.sleep(4)
##                else:
##                    print("Retrying in 3s...\n")
##                    time.sleep(3)
##            except IOError as error2:
##                print("Something went horribly wrong:")
##                print(error2)
##                if x == 2:
##                    print("Could not connect\n=================")
##                    time.sleep(4)
##                else:
##                    print("Retrying in 3s...\n")
##                    time.sleep(3)
            

