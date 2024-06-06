import asyncio
from bleak import BleakScanner, BleakClient

# import pandas as pd
from time import sleep
import matplotlib.pyplot as plt

# import liveplot_test

# SERVICE_UUID        = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
# CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

TH_UUID      = "e87917aa-103e-4d61-a11d-ae0d09963b64"
PPG_UUID     = "4703e542-0870-4fd7-8685-0dc1b371d700"
VOC_UUID     = "8c367fd0-fd14-49a8-bf73-d5dfa76f6e1a"
ECG_ACC_UUID = "b37a5a9f-97ac-4f36-9f1f-024c0a3e896d"

THName = "FYP_TH_Node"

# DeviceName = "FYP_SensorNode0"
TH_sampling_rate = 0.2
PPG_sampling_rate = 1
VOC_sampling_rate = 1
ECG_ACC_sampling_rate = 0.1

#Temperature and Humidity values
Temp = 0
Humid = 0

#PPG values
HR_val = 0
SpO2_val = 0

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
    device = await BleakScanner.find_device_by_name(THName, timeout=10.0)

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

                timestamp = float(buff.split(",")[0])
                Temp = float(buff.split(",")[1])
                Humid = float(buff.split(",")[2])

                print(f"Timestamp: {timestamp} \t Temperature: {Temp} \t Humidity: {Humid}")
    
                outputFile.write(buff)
                outputFile.write("\n")
                await asyncio.sleep(TH_sampling_rate)

async def BLErx_PPG(device, outputFile):
    global HR_val, SpO2_val
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(PPG_UUID)
                buff = buff.decode("utf-8")

                HR_val = float(buff.split(",")[0])
                SpO2_val = float(buff.split(",")[1])

                print(f"HR: {HR_val} \t SpO2: {SpO2_val}")
    
                outputFile.write(buff)
                outputFile.write("\n")
                await asyncio.sleep(PPG_sampling_rate)


async def BLErx_VOC(device, outputFile):
    global VOC_val, NO2_val
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(VOC_UUID)
                buff = buff.decode("utf-8")

                VOC_val = float(buff.split(",")[0])
                NO2_val = float(buff.split(",")[1])

                print(f"VOC: {VOC_val} \t NO2: {NO2_val}")
    
                outputFile.write(buff)
                outputFile.write("\n")
                await asyncio.sleep(VOC_sampling_rate)




# async def async_main(device, outputFile):
    
#     await asyncio.gather(BLErx(device, outputFile), plot())


    

# if __name__ == "__main__":
#     DeviceDetected = asyncio.run(BLEconnect())

#     data_file = open("Data/output.csv", "w")
#     data_file.write("Temperature, Humidity\n")

#     asyncio.run(async_main(device=DeviceDetected, outputFile=data_file))

#     data_file.close()