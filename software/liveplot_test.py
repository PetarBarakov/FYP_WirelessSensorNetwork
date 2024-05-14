import matplotlib.pyplot as plt
import matplotlib.animation as animation

import BLE_receiver
import asyncio

import time

import random

import threading



# This function is called periodically from FuncAnimation
def animate(i, tempSamples, humidSamples, timeLen, tempLine, humidLine):

    temp_c = BLE_receiver.Temp
    humid = BLE_receiver.Humid
    # Add y to list
    tempSamples.append(temp_c)
    humidSamples.append(humid)

    # Limit y list to set number of items
    tempSamples = tempSamples[-timeLen:]
    humidSamples = humidSamples[-timeLen:]

    # Update line with new Y values
    tempLine.set_ydata(tempSamples)
    humidLine.set_ydata(humidSamples)

    return tempLine, humidLine, 

def plot ():

    # Parameters
    timeLen = 200         # Number of points to display
    tempRange = [24, 32]  # Range of possible Y values to display
    humidRange = [40, 50]  # Range of possible Y values to display

    # Create figure for plotting
    fig = plt.figure()
    tempax = fig.add_subplot(1, 1, 1)
    humidax = tempax.twinx()
    timeSamples = list(range(0, 200))
    tempSamples = [0] * timeLen
    humidSamples = [0] * timeLen
    tempax.set_ylim(tempRange)
    humidax.set_ylim(humidRange)

    # Create a blank line. We will update the line in animate
    tempLine, = tempax.plot(timeSamples, tempSamples, color='blue')
    humidLine, = humidax.plot(timeSamples, humidSamples, color = 'red')

    # Add labels
    plt.title('Temperature and Hunidity')
    plt.xlabel('Samples')
    tempax.set_ylabel('Temperature (deg C)')
    humidax.set_ylabel('Humidity (%)')


    # Set up plot to call animate() function periodically
    ani = animation.FuncAnimation(fig,
        animate,
        fargs=(tempSamples, humidSamples, timeLen, tempLine, humidLine),
        interval=BLE_receiver.samplingRate*1000,
        blit=True)
    plt.show()

def start_ploting():
    plot_thread = threading.Thread(target=plot)
    plot_thread.start()


async def async_main(device, outputFile):
    
    await asyncio.gather(BLE_receiver.BLErx(device, outputFile))


if __name__ == "__main__":
    start_ploting()

    DeviceDetected = asyncio.run(BLE_receiver.BLEconnect())

    data_file = open("Data/output.csv", "w")
    data_file.write("Temperature, Humidity\n")

    asyncio.run(async_main(device=DeviceDetected, outputFile=data_file))
    # plot()

    data_file.close()
