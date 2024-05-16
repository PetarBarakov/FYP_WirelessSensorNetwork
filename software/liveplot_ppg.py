import matplotlib.pyplot as plt
import matplotlib.animation as animation

import BLE_receiver
import asyncio

import threading

# This function is called periodically from FuncAnimation
def animate(i, HRSamples, SpO2Samples, timeLen, HRLine, SpO2Line, HRRange, SpO2Range, HRax, SpO2ax):

    HR = BLE_receiver.HR_val
    SpO2 = BLE_receiver.SpO2_val
    # Add y to list
    HRSamples.append(HR)
    SpO2Samples.append(SpO2)

    # Limit y list to set number of items
    HRSamples = HRSamples[-timeLen:]
    SpO2Samples = SpO2Samples[-timeLen:]

    # Update line with new Y values
    HRLine.set_ydata(HRSamples)
    SpO2Line.set_ydata(SpO2Samples)

    # Adjust y-axis limits
    HRax.set_ylim(min(HRSamples)-100, max(HRSamples)+100)
    SpO2ax.set_ylim(min(SpO2Samples)-100, max(SpO2Samples)+100)

    return HRLine, SpO2Line, 

def plot ():

    # Parameters
    timeLen = 200         # Number of points to display
    HRRange = [2260000, 2260000]  # Range of possible Y values to display
    SpO2Range = [2700000, 2700000]  # Range of possible Y values to display

    # Create figure for plotting
    fig = plt.figure()
    HRax = fig.add_subplot(1, 1, 1)
    SpO2ax = HRax.twinx()
    timeSamples = list(range(0, timeLen))
    HRSamples = [0] * timeLen
    SpO2Samples = [0] * timeLen
    # HRax.set_ylim(HRRange)
    # SpO2ax.set_ylim(SpO2Range)

    # Create a blank line. We will update the line in animate
    HRLine, = HRax.plot(timeSamples, HRSamples, color='blue')
    SpO2Line, = SpO2ax.plot(timeSamples, SpO2Samples, color = 'red')

    # Add labels
    plt.title('PPG Data')
    plt.xlabel('SpO2 Samples')
    HRax.set_ylabel('HR')
    SpO2ax.set_ylabel('SpO2')

    plt.legend([HRLine, SpO2Line], ['HR', 'SpO2'])

    # Set up plot to call animate() function periodically
    ani = animation.FuncAnimation(fig,
        animate,
        fargs=(HRSamples, SpO2Samples, timeLen, HRLine, SpO2Line, HRRange, SpO2Range, HRax, SpO2ax),
        interval=BLE_receiver.samplingRate*1000,
        blit=True)
    plt.show()

def start_ploting():
    plot_thread = threading.Thread(target=plot)
    plot_thread.start()


async def async_main_temp(device, outputFile):
    
    await asyncio.gather(BLE_receiver.BLErx_PPG(device, outputFile))


if __name__ == "__main__":
    start_ploting()

    DeviceDetected = asyncio.run(BLE_receiver.BLEconnect())

    data_file = open("Data/output_ppg.csv", "w")
    data_file.write("HR, SpO2\n")

    asyncio.run(async_main_temp(device=DeviceDetected, outputFile=data_file))
    # plot()

    data_file.close()
