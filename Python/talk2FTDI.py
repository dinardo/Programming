"""
###################################################################
# Program to communicate with FTDI device attached to serial port #
# e.g. /dev/ttyUSB0                                               #
# To know to which port is connected the FTDI device run (linux): #
# > dmesg                                                         #
# To add user to dialout group: sudo usermod -a -G dialout <user> #
# To install -serial-: pip install pyserial                       #
#                                                                 #
# By Mauro Dinardo                                                #
###################################################################
"""

import serial
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

    sendANDreceive(device,'CURRent:PROTection:STATe ON')

    sendANDreceive(device,'VOLTage:PROTection ' + str(voltProtect))
    sendANDreceive(device,'CURRent:PROTection ' + str(currentProtect))

    sendANDreceive(device,'VOLTage:PROTection?')
    sendANDreceive(device,'CURRent:PROTection?')

def measureVandI(device):
    sendANDreceive(device,'MEASure:VOLTage?')
    sendANDreceive(device,'MEASure:CURRent?')

def setVoltage(device, voltage):
    sendANDreceive(device,'VOLTage ' + str(voltage))

def setCurrent(device, current):
    sendANDreceive(device,'CURRent ' + str(current))

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
        print('Synopsis:', sys.argv[0], '[DEVICE_NAME [ON/OFF/ADDR/FTDI/CHN]]')
        sys.exit(1)

    ######################
    # Parse output state #
    ######################
    output = ''
    if len(sys.argv) >= 3 and (sys.argv[2] == 'ON' or sys.argv[2] == 'on' or sys.argv[2] == 'OFF' or sys.argv[2] == 'off'):
        output = sys.argv[2]


    ########################
    # Parse channel number #
    ########################
    chn = ''
    if len(sys.argv) == 5 and (sys.argv[3] == 'CHN' or sys.argv[3] == 'chn'):
        chn = sys.argv[4]


    ####################
    # Open serial port #
    ####################
    print('Initializing serial port on: ' + sys.argv[1])
    device = serial.Serial(sys.argv[1], 9600, timeout=0.5)


    ####################
    # Sending commands #
    ####################
    if len(sys.argv) == 4 and (sys.argv[2] == 'ADDR' or sys.argv[2] == 'addr') and sys.argv[3].isdigit():
        #############################
        # To program FTDI chip:     #
        # talk2FTDI device ADDR num #
        #############################
        print('--> Reprogramming FTDI to talk to GPIB adevice with address:', sys.argv[3], '<--')
        sendANDreceive(device,'++addr ' + sys.argv[3])

    elif len(sys.argv) == 3 and (sys.argv[2] == 'FTDI' or sys.argv[2] == 'ftdi'):
        #########################
        # To read FTDI status:  #
        # talk2FTDI device FTDI #
        #########################
        print('--> FTDI status <--')
        FTDIstatus(device)

    else:
        #############################################
        # To read/program device status             #
        # talk2FTDI device [ON/OFF]->CHN n./nothing #
        #############################################
        sendANDreceive(device,'*IDN?')
        sendANDreceive(device,'OUTPut:STATe?')

        if chn != '':
            print('\n--> Channel #' + chn)
            sendANDreceive(device,'INSTrument:SELect OUT' + chn)
            sendANDreceive(device,'SOURce:CHANnel ' + chn)
            queryProtections(device, 2.2, 5.0)
            setCurrent(device, 4.0)
#            setVoltage(device, 1.78)
            measureVandI(device)

        else:
            print('\n--> Channel #1')
            sendANDreceive(device,'INSTrument:SELect OUT0')
            sendANDreceive(device,'SOURce:CHANnel 1')
            queryProtections(device, 1.9, 2.0)
            setVoltage(device, 1.70)
            measureVandI(device)

            #rampUpVoltage(device, 1.5, 1.85, 0.1)

            print('\n--> Channel #2')
            sendANDreceive(device,'INSTrument:SELect OUT1')
            sendANDreceive(device,'SOURce:CHANnel 2')
            queryProtections(device, 1.9, 2.0)
            setVoltage(device, 1.70)
            measureVandI(device)

            print('\n--> Channel #3')
            sendANDreceive(device,'INSTrument:SELect OUT2')
            sendANDreceive(device,'SOURce:CHANnel 3')
            queryProtections(device, 2.6, 0.6)
            setVoltage(device, 2.5)
            measureVandI(device)

        if output != '':
            sendANDreceive(device,'OUTPut:STATe ' + output)
            sendANDreceive(device,'OUTPut:STATe?')
            measureVandI(device)


if __name__ == '__main__':
    main()
