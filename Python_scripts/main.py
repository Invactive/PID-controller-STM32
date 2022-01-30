import serial #pip install pyserial
import matplotlib.pyplot as plt
import time
import csv

# Init variables/arrays
t = 0
Tp = 1                      # sampling time [s]
curr_temp_samples = []
curr_temp_flag = 0
set_temp_samples = []
set_temp_flag = 0
timebase = []
d = bytearray()             # serial read buffer

# Handling plot close event
close_flag = 1
def handle_close(event):
    global close_flag
    close_flag = 0
    print("Data logging finished!")

# Figure init
fig = plt.figure(figsize=(10,5))
fig.canvas.mpl_connect('key_press_event', handle_close)
fig.canvas.mpl_connect('close_event', handle_close)
plt.ion()

# Saving data to .csv file
timestr = time.strftime("%Y%m%d-%H%M%S")
data = open("data_{}.csv".format(timestr) , 'w', newline='')
writer = csv.writer(data, delimiter=',')
header = ["Time", "Curr_temp", "Set_temp"]
writer.writerow(header)

# For debugging
# UART = serial.Serial("COM4", 115200, timeout=1, parity=serial.PARITY_NONE)
# set_start_temp = "30.00"

# User input
COM_PORT = int(input("Serial port number: "))
UART = serial.Serial("COM{}".format(COM_PORT), 115200, timeout=1, parity=serial.PARITY_NONE)
set_start_temp = str(input("Set temperature: "))
UART.write(set_start_temp.encode())

while close_flag:
    if UART.inWaiting() >= 0:
        d += UART.read(1)
        if b"Current temperature: " in d:
            curr_temp = UART.read(5)
            curr_temp = curr_temp.decode()
            curr_temp = float(curr_temp)
            curr_temp_samples.append(curr_temp)
            d[::] = b""
            curr_temp_flag = 1
            print("Current temperature:", curr_temp)
        elif b"Set temperature: " in d:
            set_temp = UART.read(5)
            set_temp = set_temp.decode()
            set_temp = float(set_temp)
            set_temp_samples.append(set_temp)
            d[::] = b""
            set_temp_flag = 1
            print("Set temperature:", set_temp)

    if curr_temp_flag and set_temp_flag:
        # Prepare x label for plot
        timebase.append(t)
        t += Tp
        print("Timebase:", timebase)

        # Plotting data
        plt.clf()
        plt.grid(True)
        plt.plot(timebase, curr_temp_samples, '.', markersize=5, label="Current temp")
        plt.plot(timebase, set_temp_samples, '.', markersize=5, label="Set temp")
        plt.xlim(0, t + 1)
        plt.title("Current temperature plot, set temp = {}".format(set_temp))
        # plt.title("Model data, PMW duty 100%")    # Used for model identification
        plt.xlabel("Time (s)")
        plt.ylabel("Temperature (C)")
        plt.legend(loc="lower right")
        plt.show(block=False)
        fig.canvas.flush_events()
        plt.pause(0.0001)
        writer.writerow([timebase[-1], curr_temp_samples[-1], set_temp_samples[-1]])
        curr_temp_flag = 0
        set_temp_flag = 0

    if close_flag == 0:
        break

fig.savefig("Temp_plot_{}.png".format(timestr))
UART.close()
data.close()