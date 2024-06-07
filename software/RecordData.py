import asyncio
import threading
from bleak import BleakScanner, BleakClient

# import pandas as pd
from time import sleep

import matplotlib.pyplot as plt
import matplotlib.animation as animation

import BLE_receiver



TH_UUID      = "e87917aa-103e-4d61-a11d-ae0d09963b64"
PPG_UUID     = "4703e542-0870-4fd7-8685-0dc1b371d700"
VOC_UUID     = "8c367fd0-fd14-49a8-bf73-d5dfa76f6e1a"
ECG_ACC_UUID = "b37a5a9f-97ac-4f36-9f1f-024c0a3e896d"

THName = "FYP_TH_Node"
PPGName = "FYP_PPG_Node"
VOCName = "FYP_VOC_Node"
ECG_ACCName = "FYP_ECG_ACC_Node"

# DeviceName = "FYP_SensorNode0"
TH_sampling_rate = 0.15
PPG_sampling_rate = 0.9
VOC_sampling_rate = 0.9
ECG_ACC_sampling_rate = 0.9

#Temperature and Humidity values
th_timestamp = 0
Temp = 0
Humid = 0

#PPG values
PPG_timestamp = 0
HR_val = 0
SpO2_val = 0

#VOC values
VOC_timestamp = 0
VOC_val = 0
NO2_val = 0

#ECG and Accelerometer values
ECG_ACC_timestamp = 0
ECG_heart_rate = 0
X_acc = 0
Y_acc = 0
Z_acc = 0
Movment = False

async def BLEconnect(device_name):
    print(f"Scanning for {device_name}... (timeout after 10s)")
    device = await BleakScanner.find_device_by_name(device_name, timeout=10.0)

    if device:
        print("DEVCIE FOUND.....")
    else:
        print("DEVICE NOT FOUND.....")
    return device

async def BLErx_temp(device, outputFile):
    global Humid, Temp
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(TH_UUID)
                buff = buff.decode("utf-8")

                th_timestamp = float(buff.split(",")[0])
                Temp = float(buff.split(",")[1])
                Humid = float(buff.split(",")[2])

                print(f"Timestamp: {th_timestamp} \t Temperature: {Temp} \t Humidity: {Humid}")
    
                # outputFile.write(buff)
                # outputFile.write("\n")
                await asyncio.sleep(TH_sampling_rate)

async def BLErx_PPG(device, outputFile):
    global HR_val, SpO2_val
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(PPG_UUID)
                buff = buff.decode("utf-8")

                PPG_timestamp = float(buff.split(",")[0])
                HR_val = float(buff.split(",")[1])
                SpO2_val = float(buff.split(",")[2])

                print(f"Timestamp: {PPG_timestamp} \t HR: {HR_val} \t SpO2: {SpO2_val}")

                await asyncio.sleep(PPG_sampling_rate)


# async def BLErx_VOC(device, outputFile):
#     global VOC_val, NO2_val
#     if device:
#         async with BleakClient(device) as client:
#             while True:
            
#                 buff = await client.read_gatt_char(VOC_UUID)
#                 buff = buff.decode("utf-8")

#                 VOC_val = float(buff.split(",")[0])
#                 NO2_val = float(buff.split(",")[1])

#                 print(f"VOC: {VOC_val} \t NO2: {NO2_val}")
    
#                 outputFile.write(buff)
#                 outputFile.write("\n")
#                 await asyncio.sleep(VOC_sampling_rate)





# This function is called periodically from FuncAnimation
# def animate(i, tempSamples, humidSamples, timeLen, tempLine, humidLine, tempRange, humidRange, tempax, humidax):

#     temp_c = BLE_receiver.Temp
#     humid = BLE_receiver.Humid
#     # Add y to list
#     tempSamples.append(temp_c)
#     humidSamples.append(humid)

#     # Limit y list to set number of items
#     tempSamples = tempSamples[-timeLen:]
#     humidSamples = humidSamples[-timeLen:]

#     # Update line with new Y values
#     tempLine.set_ydata(tempSamples)
#     humidLine.set_ydata(humidSamples)


#     # Autoscale Y-axis
#     if(temp_c != 0 and humid != 0):
#         if(temp_c > tempRange[1]):
#             tempRange[1] = temp_c + 1
#             tempax.set_ylim(tempRange)

#         if(temp_c < tempRange[0]):
#             tempRange[0] = temp_c - 1
#             tempax.set_ylim(tempRange)

#         if(temp_c < tempRange[1] - 5):
#             tempRange[1] -= 1
#             tempax.set_ylim(tempRange)

#         if(temp_c > tempRange[0] + 5):
#             tempRange[0] += 1
#             tempax.set_ylim(tempRange)



#         if(humid > humidRange[1]):
#             humidRange[1] = humid + 1
#             humidax.set_ylim(humidRange)
            

#         if(humid < humidRange[0]):
#             humidRange[0] = humid - 1
#             humidax.set_ylim(humidRange)

#         if(humid < humidRange[1] - 5):
#             humidRange[1] -= 1
#             humidax.set_ylim(humidRange)

#         if(humid > humidRange[0] + 5):
#             humidRange[0] += 1
#             humidax.set_ylim(humidRange)


#     return tempLine, humidLine, 

# def plot ():

#     # Parameters
#     timeLen = 600         # Number of points to display
#     tempRange = [24, 26]  # Range of possible Y values to display
#     humidRange = [44, 46]  # Range of possible Y values to display

#     # Create figure for plotting
#     fig = plt.figure()
#     tempax = fig.add_subplot(1, 1, 1)
#     humidax = tempax.twinx()
#     timeSamples = list(range(0, timeLen))
#     tempSamples = [0] * timeLen
#     humidSamples = [0] * timeLen
#     tempax.set_ylim(tempRange)
#     humidax.set_ylim(humidRange)

#     # Create a blank line. We will update the line in animate
#     tempLine, = tempax.plot(timeSamples, tempSamples, color='blue')
#     humidLine, = humidax.plot(timeSamples, humidSamples, color = 'red')

#     # Add labels
#     plt.title('Temperature and Hunidity')
#     plt.xlabel('Samples')
#     tempax.set_ylabel('Temperature (deg C)')
#     humidax.set_ylabel('Humidity (%)')

#     plt.legend([tempLine, humidLine], ['Temperature', 'Humidity'])

#     # Set up plot to call animate() function periodically
#     ani = animation.FuncAnimation(fig,
#         animate,
#         fargs=(tempSamples, humidSamples, timeLen, tempLine, humidLine, tempRange, humidRange, tempax, humidax),
#         interval=BLE_receiver.samplingRate*1000,
#         blit=True)
#     plt.show()

# def start_ploting():
#     plot_thread = threading.Thread(target=plot)
#     plot_thread.start()

async def async_main_th(device, outputFile):
    await asyncio.gather(BLErx_temp(device, outputFile))

async def async_main_ppg(device, outputFile):
    await asyncio.gather(BLErx_PPG(device, outputFile))


def async_entry_th(device, outputFile):
    asyncio.run(async_main_th(device, outputFile))

def async_entry_ppg(device, outputFile):
    asyncio.run(async_main_ppg(device, outputFile))


def init_ble_threads(devices, outputFile):
    th_thread = threading.Thread(target=async_entry_th, args=(devices[0], outputFile,))
    ppg_thread = threading.Thread(target=async_entry_ppg, args=(devices[1], outputFile,))
    ppg_thread.start()
    th_thread.start()



if __name__ == "__main__":
    # start_ploting()

    th_device  = asyncio.run(BLEconnect(THName))
    ppg_device = asyncio.run(BLEconnect(PPGName))

    data_file = open("Data/multinodal_readout.csv", "w")
    data_file.write("TH Timstamp, Temperature, Humidity,, PPG Timestamp, PPG HR, Sp02\n")

    init_ble_threads(devices=[th_device, ppg_device], outputFile=data_file)
    # plot()

    data_file.close()







