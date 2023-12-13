import serial 
import argparse
import sys

encoding = 'utf-8'
parser = argparse.ArgumentParser(description='Pipe file in serial interface')
parser.add_argument('-f', '--file', type=str, help='Path to the exploit file.', required=True)
args = parser.parse_args()


try:
    interface = serial.Serial("/dev/ttyACM0", 9600)
except:
    print("Was not able to open interface")
    sys.exit()

print(interface)

with open(args.file, mode='rb') as file: 
    fileContent = file.read()
    content_str = ""
    for elem in fileContent:
        content_str += hex(elem) + " "
    print("writing:\n"+content_str)
    interface.write(fileContent)

interface.close()
