import time
import serial
import sys
from KB import KBHit
print("Example:\n\tL35 sends 35% to lamp.\n\tT35 sends 35% to fan.\n\"Enter port's full name, E: COM3\"")
port = input("Enter port:")
# port="COM2"

# init serial port
try:
    serial_port=serial.Serial(port, baudrate=9600)
except Exception as err:
    print("Error: Could not open the serial port")
    print(err)
    exit(1)
    
kb = KBHit()
    
input_buffer = ""
output_buffer = ""

# def send_procedure(buffer) :
#     serial_port.write(buffer.encode()+b'\r\n')
#     # clear screen ? 
#     print("\n>>"+buffer)
# #     buffer=""
# #     print("Buffer at the end:",buffer)

while(1):
    if kb.kbhit():
        c=kb.getch()
        output_buffer+=c
        if ord(c)==27: #Esc
            serial_port.close()
            break
        elif ord(c)==13: #newline
#             send_procedure(output_buffer)
            serial_port.write(output_buffer.encode()+b'\r\n')
            print("\n<<"+output_buffer)
            output_buffer=""
#             print("Buffer at the end:",output_buffer)
        print("Buffer ::",output_buffer)
    
    # receiving and printing
    while serial_port.inWaiting() > 0:
#         input_buffer += serial_port.readline().decode()
        input_buffer += serial_port.read().decode()
#     time.sleep(0.5)
    if len(input_buffer)>0 and input_buffer[-1] == "\n" :
        print(">>",input_buffer)
        input_buffer=""


