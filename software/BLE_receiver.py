import asyncio
from bleak import BleakScanner, BleakClient

# import pandas as pd
from time import sleep

# import liveplot_test
import matplotlib.pyplot as plt


SERVICE_UUID        = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

DeviceName = "FYP_SensorNode0"
samplingRate = 0.01 # 2 second sampling rate

sample_array = []
temp_array = []
humid_array = []



def plotter(ax1, ax2, sample_indx, temp, humid):
    print(f"Temperature: {temp} \t Humidity: {humid}")

    sample_array.append(sample_indx)
    temp_array.append(temp)
    humid_array.append(humid)

    # Update the plot
    ax1.clear()
    ax2.clear()

    ax1.plot(sample_array, temp_array, label="Temperature", color="blue")
    ax2.plot(sample_array, humid_array, label="Humidity", color="red")
    plt.title("Temperature over Time")
    ax1.set_xlabel("Samples")
    ax1.set_ylabel("Temperature (deg C)")
    ax2.set_ylabel("Humidity (%)")

    plt.pause(samplingRate)

    if(len(sample_array) > 200):
        sample_array.pop(0)
        temp_array.pop(0)
        humid_array.pop(0)
                    

async def BLEconnect():
    print("Scanning for device... (timeout after 10s)")
    device = await BleakScanner.find_device_by_name(DeviceName, timeout=10.0)

    if device:
        print("DEVCIE FOUND.....")
    else:
        print("DEVICE NOT FOUND.....")
    return device

async def BLErx(device, outputFile):
    if device:
        async with BleakClient(device) as client:
            
            sample_indx = 0

            fig, ax1 = plt.subplots()
            ax2 = ax1.twinx()

            while True:
                TempHumid = await client.read_gatt_char(CHARACTERISTIC_UUID)
                TempHumid = TempHumid.decode("utf-8")

                Temp = float(TempHumid.split(",")[0])
                Humid = float(TempHumid.split(",")[1])

                
                outputFile.write(TempHumid)
                outputFile.write("\n")

                plt.ion()
                fig , ax1 = plt.subplots()
                ax2 = ax1.twinx()

                #plot
                plotter(ax1, ax2, sample_indx, Temp, Humid)
                sample_indx += 1

                                
                # sleep(samplingRate)

    

if __name__ == "__main__":
    DeviceDetected = asyncio.run(BLEconnect())

    data_file = open("Data/output.csv", "w")
    data_file.write("Temperature, Humidity\n")

    asyncio.run(BLErx(device=DeviceDetected, outputFile=data_file))

    data_file.close()