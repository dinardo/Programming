import subprocess
import sys

def main():
    parameters = {
    'Par1' : ['-p', 'par1'],
    'Par2' : ['-p', 'par2']
    }

    proc = subprocess.Popen(['python', 'WakeOnLAN.py'] + parameters['Par1'], stdout=subprocess.PIPE)
    print(proc)

    while True:
        lineBytes = proc.stdout.readline()

        if not lineBytes:
            break

        line = str(lineBytes)
        sys.stdout.buffer.write(lineBytes)
        sys.stdout.flush()
        match = line.find('example')

        if match != -1:
            print('I found it!')

    print('Exit')

if __name__ == '__main__':
    main()
