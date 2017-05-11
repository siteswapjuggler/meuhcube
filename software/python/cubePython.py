import serial, time
ser = serial.Serial('/dev/tty.wchusbserial1410', 115200)

#need to rest for the setup
time.sleep(1)

ser.write(b"\xE7\x10\x00\x7E")
time.sleep(1)
ser.write(b"\xE7\x10\x00\x7E")
time.sleep(1)
