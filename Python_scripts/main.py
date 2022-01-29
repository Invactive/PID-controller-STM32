import numpy as np
import serial
import matplotlib.pyplot as plt
import time
import re

Tp = 1 # sampling time [s]
temp_samples = []
timebase = []
while(1):
    UART = serial.Serial('COM4', 115200, timeout=1, parity=serial.PARITY_NONE)
    line = UART.readline()
    line = line.decode("utf-8")
    line = re.findall(r'\d{1,2}.\d{1,2}',line)
    temp_samples.append(float(line[0]))
    print(temp_samples)
    line = UART.readline()
    print(line)
    UART.close()
    time.sleep(Tp)










