""" This is a test file to check understanding of python embedding in C++ via
    only including Python.h in the include folder of the python 2.7 installation.
    Jonathan Holland - s4265832 - METR4810
    Bluetooth Comms                                                                 """

def testFunction():
    c = 12345*6789
    print 'The result of 12345 x 6789 :', c
    return c
