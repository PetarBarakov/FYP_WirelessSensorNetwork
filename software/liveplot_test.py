import matplotlib.pyplot as plt
import matplotlib.animation as animation
import random

# Parameters
timeLen = 200         # Number of points to display
tempRange = [10, 40]  # Range of possible Y values to display
humidRange = [15, 30]  # Range of possible Y values to display

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

def read_temp():
    temp_c = random.randint(10, 40)

    return temp_c

def read_humid():
    humid = random.randint(20, 25)

    return humid

# This function is called periodically from FuncAnimation
def animate(i, tempSamples, humidSamples):


    temp_c = read_temp()
    humid = read_humid()
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


# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig,
    animate,
    fargs=(tempSamples, humidSamples),
    interval=50,
    blit=True)
plt.show()