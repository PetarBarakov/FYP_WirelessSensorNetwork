import asyncio
from bleak import BleakScanner, BleakClient

# import pandas as pd
from time import sleep
import matplotlib.pyplot as plt

# import liveplot_test

SERVICE_UUID        = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

DeviceName = "FYP_SensorNode0"
samplingRate = 0.1

#Temperature and Humidity values
Temp = 0
Humid = 0

#PPG values
IR_val = 0
RED_val = 0

#VOC values
VOC_val = 0
NO2_val = 0

#ECG values


#Accelerometer values
X_acc =0
Y_acc =0
Z_acc =0

async def BLEconnect():
    print("Scanning for device... (timeout after 10s)")
    device = await BleakScanner.find_device_by_name(DeviceName, timeout=10.0)

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
            
                buff = await client.read_gatt_char(CHARACTERISTIC_UUID)
                buff = buff.decode("utf-8")

                Temp = float(buff.split(",")[0])
                Humid = float(buff.split(",")[1])

                print(f"Temperature: {Temp} \t Humidity: {Humid}")
    
                outputFile.write(buff)
                outputFile.write("\n")
                # await asyncio.sleep(samplingRate)

async def BLErx_PPG(device, outputFile):
    global IR_val, RED_val
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(CHARACTERISTIC_UUID)
                buff = buff.decode("utf-8")

                IR_val = float(buff.split(",")[0])
                RED_val = float(buff.split(",")[1])

                print(f"IR: {IR_val} \t Red: {RED_val}")
    
                outputFile.write(buff)
                outputFile.write("\n")
                await asyncio.sleep(samplingRate)


async def BLErx_VOC(device, outputFile):
    global VOC_val, NO2_val
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(CHARACTERISTIC_UUID)
                buff = buff.decode("utf-8")

                VOC_val = float(buff.split(",")[0])
                NO2_val = float(buff.split(",")[1])

                print(f"VOC: {VOC_val} \t NO2: {NO2_val}")
    
                outputFile.write(buff)
                outputFile.write("\n")
                await asyncio.sleep(samplingRate)




# async def async_main(device, outputFile):
    
#     await asyncio.gather(BLErx(device, outputFile), plot())


    

# if __name__ == "__main__":
#     DeviceDetected = asyncio.run(BLEconnect())

#     data_file = open("Data/output.csv", "w")
#     data_file.write("Temperature, Humidity\n")

#     asyncio.run(async_main(device=DeviceDetected, outputFile=data_file))

#     data_file.close()