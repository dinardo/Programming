"""
###################################################################
# Program to communicate with FTDI device attached to serial port #
# e.g. /dev/ttyUSB0                                               #
# To know to which port is connected the FTDI device run (linux): #
# > dmesg                                                         #
# To add user to dialout group: sudo usermod -a -G dialout <user> #
#                                                                 #
# By Mauro Dinardo                                                #
###################################################################
"""

import serial # --> needs pyserial
import sys


def sendANDreceive(device, cmd):
    print('\nSending:', cmd)
    device.write(str.encode(cmd + '\n'))
    out = device.read(256);
    if len(out) > 0:
        print('Received:', out)


def queryProtections(device, voltProtect, currentProtect):
    sendANDreceive(device,'VOLTage:PROTection:STATe?')
    sendANDreceive(device,'CURRent:PROTection:STATe?')

    sendANDreceive(device,'VOLTage:PROTection:TRIPped?')
    sendANDreceive(device,'CURRent:PROTection:TRIPped?')

    sendANDreceive(device,'VOLTage:PROTection:CLEar')
    sendANDreceive(device,'CURRent:PROTection:CLEar')

    sendANDreceive(device,'CURRent:PROTection:STATe 1')

    sendANDreceive(device,'VOLTage:PROTection ' + str(voltProtect))
    sendANDreceive(device,'CURRent:PROTection ' + str(currentProtect))
    sendANDreceive(device,'CURRent ' + str(currentProtect))

    sendANDreceive(device,'VOLTage:PROTection?')
    sendANDreceive(device,'CURRent:PROTection?')

    sendANDreceive(device,'MEASure:VOLTage?')
    sendANDreceive(device,'MEASure:CURRent?')

def setVoltage(device, voltage):
    sendANDreceive(device,'VOLTage ' + str(voltage))
    sendANDreceive(device,'MEASure:VOLTage?')
    sendANDreceive(device,'MEASure:CURRent?')


def rampUpVoltage(device, start, stop, step):
    print('\n--> Ramping up voltage: from', start, 'to', stop, 'in step of', step, 'V')
    sendANDreceive(device,'VOLTage ' + str(start))
    sendANDreceive(device,'VOLTage:STEP ' + str(step))

    volt = start
    for i in range(int((stop - start) / step)):
        volt += step
        if volt > stop:
            break
        print('Volatge -->', volt)
        #sendANDreceive(device,'VOLTage Up')
        sendANDreceive(device,'VOLTage ' + str(volt))


def FTDIstatus(device):
    sendANDreceive(device,'++addr')
    sendANDreceive(device,'++mode')
    sendANDreceive(device,'++ver')
    sendANDreceive(device,'++srq')
    sendANDreceive(device,'++savecfg')
    sendANDreceive(device,'++eot_char')
    sendANDreceive(device,'++eot_enable')
    sendANDreceive(device,'++eos')
    sendANDreceive(device,'++eoi')


def main():
    if len(sys.argv) < 2:
        print('Synopsis:', sys.argv[0], '[DEVICE_NAME [ON/OFF/ADDR/FTDI]]')
        sys.exit(1)


    ####################
    # Open serial port #
    ####################
    print('Initializing serial port on: ' + sys.argv[1])
    device = serial.Serial(sys.argv[1], 9600, timeout=0.5)


    ####################
    # Sending commands #
    ####################
    if len(sys.argv) == 3 and sys.argv[2].isdigit():
        ########################
        # To program FTDI chip #
        ########################
        print('--> Reprogramming FTDI to talk to GPIB adevice with address:', sys.argv[2], '<--')
        sendANDreceive(device,'++addr ' + sys.argv[2])

    elif len(sys.argv) == 3 and (sys.argv[2] == 'FTDI' or sys.argv[2] == 'ftdi'):
        #######################
        # To read FTDI status #
        #######################
        print('--> FTDI status <--')
        FTDIstatus(device)

    else:
        #################################
        # To read/program device status #
        #################################
        sendANDreceive(device,'*IDN?')
        sendANDreceive(device,'OUTPut:STATe?')

        print('\n--> Channel #1')
        sendANDreceive(device,'INSTrument:SELect OUT0')
        sendANDreceive(device,'SOURce:CHANnel 1')
        queryProtections(device, 1.9, 1.2)
        setVoltage(device, 1.85)

        #rampUpVoltage(device, 1.5, 1.85, 0.1)

        print('\n--> Channel #2')
        sendANDreceive(device,'INSTrument:SELect OUT1')
        sendANDreceive(device,'SOURce:CHANnel 2')
        queryProtections(device, 1.9, 1.2)
        setVoltage(device, 1.85)

        print('\n--> Channel #3')
        sendANDreceive(device,'INSTrument:SELect OUT2')
        sendANDreceive(device,'SOURce:CHANnel 3')
        queryProtections(device, 2.6, 0.6)
        setVoltage(device, 2.5)

        if len(sys.argv) == 3:
            sendANDreceive(device,'OUTPut:STATe ' + sys.argv[2])
            sendANDreceive(device,'OUTPut:STATe?')


if __name__ == '__main__':
    main()
