from time import sleep
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import serial
import colorsys

# Open the serial port
ser = serial.Serial('/dev/ttyACM0', 9600)  # replace '/dev/ttyACM0' with your serial port
fig = plt.figure()
ax = fig.add_subplot(111, polar=True)

#open plot (not blocking)
# plt.ion()
# plt.show()

while True:
    # Read a line from the serial port
    line = ser.readline().decode('utf-8').strip()

    try:
        # Split the line into RGB values
        red, green, blue = map(int, line.split(','))
    except:
        continue

    # convert rgb to xy (hsv = hue saturation value)
    hsv = colorsys.rgb_to_hsv(red/30000, green/30000, blue/30000)

    # plot hue and saturation in a 2d polar plot

    # ax.plot(hsv[0]*2*3.14159, hsv[1], 'ro')
    # # redraw the plot
    # plt.draw()
    print(f"hue: {hsv[0]*2*3.14159}, saturation: {hsv[1]}")
