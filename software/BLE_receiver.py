import asyncio
from bleak import BleakScanner, BleakClient

# import pandas as pd
from time import sleep

# import liveplot_test

SERVICE_UUID        = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

DeviceName = "FYP_SensorNode0"
samplingRate = 2 # 2 second sampling rate

Temp = 0
Humid = 0

async def BLEconnect():
    print("Scanning for device... (timeout after 10s)")
    device = await BleakScanner.find_device_by_name(DeviceName, timeout=10.0)

    if device:
        print("DEVCIE FOUND.....")
    else:
        print("DEVICE NOT FOUND.....")
    return device

async def TempHumid():
    global Humid, Temp
    while True:
        await asyncio.sleep(samplingRate)
        print(f"1:Temperature: {Temp} \t Humidity: {Humid}")

async def BLErx(device, outputFile):
    global Humid, Temp
    if device:
        async with BleakClient(device) as client:
            while True:
            
                TempHumid = await client.read_gatt_char(CHARACTERISTIC_UUID)
                TempHumid = TempHumid.decode("utf-8")

                Temp = float(TempHumid.split(",")[0])
                Humid = float(TempHumid.split(",")[1])

                print(f"2:Temperature: {Temp} \t Humidity: {Humid}")
    
                outputFile.write(TempHumid)
                outputFile.write("\n")
                await asyncio.sleep(samplingRate)

                # return Temp, Humid

async def async_main(device, outputFile):
    
    await asyncio.gather(BLErx(device, outputFile), TempHumid())


    

if __name__ == "__main__":
    DeviceDetected = asyncio.run(BLEconnect())

    data_file = open("Data/output.csv", "w")
    data_file.write("Temperature, Humidity\n")

    asyncio.run(async_main(device=DeviceDetected, outputFile=data_file))

    data_file.close()