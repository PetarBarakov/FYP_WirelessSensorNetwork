import matplotlib.pyplot as plt
import matplotlib.animation as animation

import BLE_receiver
import asyncio

import threading

# This function is called periodically from FuncAnimation
def animate(i, IRSamples, RedSamples, timeLen, IRLine, RedLine, IRRange, RedRange, irax, redax):

    ir = BLE_receiver.IR_val
    red = BLE_receiver.RED_val
    # Add y to list
    IRSamples.append(ir)
    RedSamples.append(red)

    # Limit y list to set number of items
    IRSamples = IRSamples[-timeLen:]
    RedSamples = RedSamples[-timeLen:]

    # Update line with new Y values
    IRLine.set_ydata(IRSamples)
    RedLine.set_ydata(RedSamples)

    # Adjust y-axis limits
    irax.set_ylim(min(IRSamples)-100, max(IRSamples)+100)
    redax.set_ylim(min(RedSamples)-100, max(RedSamples)+100)


    # # Autoscale Y-axis
    # if(ir != 0 and red != 0):
    #     if(ir > IRRange[1]):
    #         IRRange[1] = ir + 10000
    #         irax.set_ylim(IRRange)

    #     if(ir < IRRange[0]):
    #         IRRange[0] = ir - 10000
    #         irax.set_ylim(IRRange)

    #     if(ir < IRRange[1] - 50000):
    #         IRRange[1] -= 10000
    #         irax.set_ylim(IRRange)

    #     if(ir > IRRange[0] + 50000):
    #         IRRange[0] += 10000
    #         irax.set_ylim(IRRange)



    #     if(red > RedRange[1]):
    #         RedRange[1] = red + 10000
    #         redax.set_ylim(RedRange)
            

    #     if(red < RedRange[0]):
    #         RedRange[0] = red - 10000
    #         redax.set_ylim(RedRange)

    #     if(red < RedRange[1] - 50):
    #         RedRange[1] -= 10000
    #         redax.set_ylim(RedRange)

    #     if(red > RedRange[0] + 50):
    #         RedRange[0] += 10000
    #         redax.set_ylim(RedRange)


    return IRLine, RedLine, 

def plot ():

    # Parameters
    timeLen = 2000         # Number of points to display
    IRRange = [2260000, 2260000]  # Range of possible Y values to display
    RedRange = [2700000, 2700000]  # Range of possible Y values to display

    # Create figure for plotting
    fig = plt.figure()
    irax = fig.add_subplot(1, 1, 1)
    redax = irax.twinx()
    timeSamples = list(range(0, timeLen))
    IRSamples = [0] * timeLen
    RedSamples = [0] * timeLen
    # irax.set_ylim(IRRange)
    # redax.set_ylim(RedRange)

    # Create a blank line. We will update the line in animate
    IRLine, = irax.plot(timeSamples, IRSamples, color='blue')
    RedLine, = redax.plot(timeSamples, RedSamples, color = 'red')

    # Add labels
    plt.title('PPG Data')
    plt.xlabel('Red Samples')
    irax.set_ylabel('IR')
    redax.set_ylabel('RED')

    plt.legend([IRLine, RedLine], ['IR', 'RED'])

    # Set up plot to call animate() function periodically
    ani = animation.FuncAnimation(fig,
        animate,
        fargs=(IRSamples, RedSamples, timeLen, IRLine, RedLine, IRRange, RedRange, irax, redax),
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
    data_file.write("IR, Red\n")

    asyncio.run(async_main_temp(device=DeviceDetected, outputFile=data_file))
    # plot()

    data_file.close()
