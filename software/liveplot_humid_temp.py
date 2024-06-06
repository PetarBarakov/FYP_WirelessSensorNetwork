import matplotlib.pyplot as plt
import matplotlib.animation as animation

import BLE_receiver
import asyncio

import threading

# This function is called periodically from FuncAnimation
def animate(i, tempSamples, humidSamples, timeLen, tempLine, humidLine, tempRange, humidRange, tempax, humidax):

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


    # Autoscale Y-axis
    if(temp_c != 0 and humid != 0):
        if(temp_c > tempRange[1]):
            tempRange[1] = temp_c + 1
            tempax.set_ylim(tempRange)

        if(temp_c < tempRange[0]):
            tempRange[0] = temp_c - 1
            tempax.set_ylim(tempRange)

        if(temp_c < tempRange[1] - 5):
            tempRange[1] -= 1
            tempax.set_ylim(tempRange)

        if(temp_c > tempRange[0] + 5):
            tempRange[0] += 1
            tempax.set_ylim(tempRange)



        if(humid > humidRange[1]):
            humidRange[1] = humid + 1
            humidax.set_ylim(humidRange)
            

        if(humid < humidRange[0]):
            humidRange[0] = humid - 1
            humidax.set_ylim(humidRange)

        if(humid < humidRange[1] - 5):
            humidRange[1] -= 1
            humidax.set_ylim(humidRange)

        if(humid > humidRange[0] + 5):
            humidRange[0] += 1
            humidax.set_ylim(humidRange)


    return tempLine, humidLine, 

def plot ():

    # Parameters
    timeLen = 600         # Number of points to display
    tempRange = [24, 26]  # Range of possible Y values to display
    humidRange = [44, 46]  # Range of possible Y values to display

    # Create figure for plotting
    fig = plt.figure()
    tempax = fig.add_subplot(1, 1, 1)
    humidax = tempax.twinx()
    timeSamples = list(range(0, timeLen))
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

    plt.legend([tempLine, humidLine], ['Temperature', 'Humidity'])

    # Set up plot to call animate() function periodically
    ani = animation.FuncAnimation(fig,
        animate,
        fargs=(tempSamples, humidSamples, timeLen, tempLine, humidLine, tempRange, humidRange, tempax, humidax),
        interval=BLE_receiver.samplingRate*1000,
        blit=True)
    plt.show()

def start_ploting():
    plot_thread = threading.Thread(target=plot)
    plot_thread.start()


async def async_main_temp(device, outputFile):
    
    await asyncio.gather(BLE_receiver.BLErx_temp(device, outputFile))


if __name__ == "__main__":
    start_ploting()

    DeviceDetected = asyncio.run(BLE_receiver.BLEconnect())

    data_file = open("Data/th.csv", "w")
    data_file.write("Timstamp, Temperature, Humidity\n")

    asyncio.run(async_main_temp(device=DeviceDetected, outputFile=data_file))
    # plot()

    data_file.close()
