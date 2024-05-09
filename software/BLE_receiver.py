import asyncio
from time import sleep

from bleak import BleakScanner, BleakClient

SERVICE_UUID        = "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

DeviceName = "FYP_SensorNode0"
samplingRate = 2 # 2 second sampling rate

def haha():
    print("HAHA")

async def BLEconnect():
    print("Scanning for device... (timeout after 10s)")
    device = await BleakScanner.find_device_by_name(DeviceName, timeout=10.0)

    if device:
        print("DEVCIE FOUND.....")
    else:
        print("DEVICE NOT FOUND.....")
    return device



async def BLErx(device):
    if device:
        async with BleakClient(device) as client:
            TempHumid = await client.read_gatt_char(CHARACTERISTIC_UUID)
            TempHumid = TempHumid.decode("utf-8")

            print(f"Temperature, Humidity: {TempHumid}")

    

if __name__ == "__main__":
    DeviceDetected = asyncio.run(BLEconnect())

    while DeviceDetected:
        asyncio.run(BLErx(device=DeviceDetected))
        haha()
        sleep(samplingRate)