import asyncio
import threading
from bleak import BleakScanner, BleakClient

# import pandas as pd
from time import sleep

import matplotlib.pyplot as plt
import matplotlib.animation as animation

# ==================== BLE Setup ====================

TH_UUID      = "e87917aa-103e-4d61-a11d-ae0d09963b64"
PPG_UUID     = "4703e542-0870-4fd7-8685-0dc1b371d700"
VOC_UUID     = "8c367fd0-fd14-49a8-bf73-d5dfa76f6e1a"
ECG_ACC_UUID = "b37a5a9f-97ac-4f36-9f1f-024c0a3e896d"

THName = "FYP_TH_Node"
PPGName = "FYP_PPG_Node"
VOCName = "FYP_VOC_Node"
ECG_ACCName = "FYP_ECG_ACC_Node"

# DeviceName = "FYP_SensorNode0"
TH_sampling_rate = 0.2
PPG_sampling_rate = 1
VOC_sampling_rate = 1
ECG_ACC_sampling_rate = 1

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
NOX_val = 0
VOC_jump = 0
NOX_jump = 0

#ECG and Accelerometer values
ECG_ACC_timestamp = 0
ECG_heart_rate = 0
X_acc = 0
Y_acc = 0
Z_acc = 0
Movement = False

async def BLEconnect(device_name):
    print(f"Scanning for {device_name}... (timeout after 10s)")
    device = await BleakScanner.find_device_by_name(device_name, timeout=10.0)

    if device:
        print("DEVICE FOUND.....")
    else:
        print("DEVICE NOT FOUND.....")
    return device

async def BLErx_temp(device):
    global th_timestamp, Humid, Temp
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(TH_UUID)
                buff = buff.decode("utf-8")

                th_timestamp = float(buff.split(",")[0])
                Temp = float(buff.split(",")[1])
                Humid = float(buff.split(",")[2])

                print(f"Timestamp: {th_timestamp} \t Temperature: {Temp} \t Humidity: {Humid}")

                await asyncio.sleep(TH_sampling_rate/2)

async def BLErx_PPG(device):
    global PPG_timestamp, HR_val, SpO2_val
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(PPG_UUID)
                buff = buff.decode("utf-8")

                PPG_timestamp = float(buff.split(",")[0])
                HR_val = float(buff.split(",")[1])
                SpO2_val = float(buff.split(",")[2])

                print(f"Timestamp: {PPG_timestamp} \t HR: {HR_val} \t SpO2: {SpO2_val}")

                await asyncio.sleep(PPG_sampling_rate/2)


async def BLErx_VOC(device):
    global VOC_timestamp, VOC_val, NOX_val, VOC_jump, NOX_jump
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(VOC_UUID)
                buff = buff.decode("utf-8")

                VOC_timestamp = float(buff.split(",")[0])
                VOC_val = float(buff.split(",")[1])
                NOX_val = float(buff.split(",")[2])
                VOC_jump = float(buff.split(",")[3])
                NOX_jump = float(buff.split(",")[4])

                print(f"Timestamp: {VOC_timestamp} \t VOC: {VOC_val} \t NO2: {NOX_val} \t VOC Jump: {VOC_jump} \t NO2 Jump: {NOX_jump}")

                await asyncio.sleep(VOC_sampling_rate/2)

async def BLErx_ECG_ACC(device):
    global ECG_ACC_timestamp, ECG_heart_rate, X_acc, Y_acc, Z_acc, Movement
    if device:
        async with BleakClient(device) as client:
            while True:
            
                buff = await client.read_gatt_char(ECG_ACC_UUID)
                buff = buff.decode("utf-8")

                ECG_ACC_timestamp = float(buff.split(",")[0])
                ECG_heart_rate = float(buff.split(",")[1])
                X_acc = float(buff.split(",")[2])
                Y_acc = float(buff.split(",")[3])
                Z_acc = float(buff.split(",")[4])
                Movement = float(buff.split(",")[5])

                print(f"Timestamp: {ECG_ACC_timestamp} \t ECG_HR: {ECG_heart_rate} \t X_acc: {X_acc} \t Y_acc: {Y_acc} \t Z_acc: {Z_acc} \t Mov: {Movement}")

                await asyncio.sleep(ECG_ACC_sampling_rate/2)



async def async_main_th(device):
    await asyncio.gather(BLErx_temp(device))

async def async_main_ppg(device):
    await asyncio.gather(BLErx_PPG(device))

async def async_main_voc(device):
    await asyncio.gather(BLErx_VOC(device))

async def async_main_ecg_acc(device):
    await asyncio.gather(BLErx_ECG_ACC(device))


def async_entry_th(device):
    asyncio.run(async_main_th(device))

def async_entry_ppg(device):
    asyncio.run(async_main_ppg(device))

def async_entry_voc(device):
    asyncio.run(async_main_voc(device))

def async_entry_ecg_acc(device):
    asyncio.run(async_main_ecg_acc(device))

def init_ble_threads(devices):
    th_thread = threading.Thread(target=async_entry_th, args=(devices[0],))
    ppg_thread = threading.Thread(target=async_entry_ppg, args=(devices[1],))
    voc_thread = threading.Thread(target=async_entry_voc, args=(devices[2],))
    ecg_acc_thread = threading.Thread(target=async_entry_ecg_acc, args=(devices[3],))

    ppg_thread.start()
    th_thread.start()
    voc_thread.start()
    ecg_acc_thread.start()


# =========================================================================
# =========================================================================
# =========================================================================
# =========================================================================
# ==================== Data Ploting and Data recording ====================

# General Values
x_num_of_seconds = 45
temp_x_num_of_samples = round(x_num_of_seconds / TH_sampling_rate)
ppg_x_num_of_samples = round(x_num_of_seconds / PPG_sampling_rate)
ecg_acc_x_num_of_samples = round(x_num_of_seconds / ECG_ACC_sampling_rate)
voc_x_num_of_samples = round(x_num_of_seconds / VOC_sampling_rate)


#Temperature and Humidity values
tempSamples = [0]*temp_x_num_of_samples
humidSamples = [0]*temp_x_num_of_samples
tempTimeSamples = [0]*temp_x_num_of_samples

#PPG and ECG values
ppgHrSamples = [0]*ppg_x_num_of_samples
ppgSpO2Samples = [0]*ppg_x_num_of_samples
ppgTimeSamples = [0]*ppg_x_num_of_samples
ecgHrSamples = [0]*ecg_acc_x_num_of_samples
ecgAccTimeSamples = [0]*ecg_acc_x_num_of_samples

#VOC values
vocSamples = [0]*voc_x_num_of_samples
noxSamples = [0]*voc_x_num_of_samples
vocTimeSamples = [0]*voc_x_num_of_samples

#Jump values
vocJumpSamples = [0]*voc_x_num_of_samples
noxJumpSamples = [0]*voc_x_num_of_samples
movJumpSamples = [0]*ecg_acc_x_num_of_samples



# This function is called periodically from FuncAnimation
def animate(i,data_file,
                Temp_Line,Humid_Line, temp_ax, humid_ax,
                PPG_HR_Line, SpO2_Line, ECG_HR_Line, hr_ax, spo2_ax,
                VOC_Line, NOX_Line, voc_ax, nox_ax,
                VOC_jump_Line, NOX_jump_Line, mov_jump_Line, jump_ax,
            ):

    global tempSamples, humidSamples, tempTimeSamples
    global th_timestamp, Temp, Humid

    global ppgHrSamples, ppgSpO2Samples, ppgTimeSamples, ecgHrSamples, ecgAccTimeSamples
    global PPG_timestamp, HR_val, SpO2_val

    global vocSamples, noxSamples, vocTimeSamples, vocJumpSamples, noxJumpSamples, movJumpSamples
    global VOC_timestamp, VOC_val, NOX_val, VOC_jump, NOX_jump, ECG_ACC_timestamp, ECG_heart_rate, X_acc, Y_acc, Z_acc, Movement

    #===========================================
    #+++++++++++++++ [0x0] +++++++++++++++++++++
    tempSamples.append(Temp)
    humidSamples.append(Humid)
    tempTimeSamples.append(th_timestamp/1000) #shows in seconds

    # Limit y list to set number of items
    tempSamples = tempSamples[-temp_x_num_of_samples:]
    humidSamples = humidSamples[-temp_x_num_of_samples:]
    tempTimeSamples = tempTimeSamples[-temp_x_num_of_samples:]

    # Update line with new Y values
    Temp_Line.set_ydata(tempSamples)
    Temp_Line.set_xdata(tempTimeSamples)
    Humid_Line.set_ydata(humidSamples)
    Humid_Line.set_xdata(tempTimeSamples)

    temp_ax.set_xlim (min(tempTimeSamples) - 0.01, max(tempTimeSamples) + 0.01)
    humid_ax.set_xlim(min(tempTimeSamples) - 0.01, max(tempTimeSamples) + 0.01)
    temp_ax.set_ylim (min(tempSamples)  - 2, max(tempSamples)  + 2)
    humid_ax.set_ylim(min(humidSamples) - 3, max(humidSamples) + 2)



    #===========================================
    #+++++++++++++++ [1x0] +++++++++++++++++++++
    ppgHrSamples.append(HR_val)
    ppgSpO2Samples.append(SpO2_val)
    ppgTimeSamples.append(PPG_timestamp/1000) #shows in seconds
    ecgHrSamples.append(ECG_heart_rate)
    ecgAccTimeSamples.append(ECG_ACC_timestamp/1000) #shows in seconds

    # Limit y list to set number of items
    ppgHrSamples = ppgHrSamples[-ppg_x_num_of_samples:]
    ppgSpO2Samples = ppgSpO2Samples[-ppg_x_num_of_samples:]
    ppgTimeSamples = ppgTimeSamples[-ppg_x_num_of_samples:]
    ecgHrSamples = ecgHrSamples[-ecg_acc_x_num_of_samples:]
    ecgAccTimeSamples = ecgAccTimeSamples[-ecg_acc_x_num_of_samples:]

    # Update line with new Y values
    PPG_HR_Line.set_ydata(ppgHrSamples)
    PPG_HR_Line.set_xdata(ppgTimeSamples)
    SpO2_Line.set_ydata(ppgSpO2Samples)
    SpO2_Line.set_xdata(ppgTimeSamples)
    ECG_HR_Line.set_ydata(ecgHrSamples)
    ECG_HR_Line.set_xdata(ecgAccTimeSamples)

    hr_ax.set_xlim (min(ppgTimeSamples) - 0.01, max(ppgTimeSamples) + 0.01)
    spo2_ax.set_xlim(min(ppgTimeSamples) - 0.01, max(ppgTimeSamples) + 0.01)
    hr_ax.set_ylim (0, 150)
    spo2_ax.set_ylim(20, 110)


    #===========================================
    #+++++++++++++++ [0x1] +++++++++++++++++++++

    vocSamples.append(VOC_val)
    noxSamples.append(NOX_val)
    vocTimeSamples.append(VOC_timestamp/1000) #shows in seconds

    vocSamples = vocSamples[-voc_x_num_of_samples:]
    noxSamples = noxSamples[-voc_x_num_of_samples:]
    vocTimeSamples = vocTimeSamples[-voc_x_num_of_samples:]

    VOC_Line.set_ydata(vocSamples)
    VOC_Line.set_xdata(vocTimeSamples)
    NOX_Line.set_ydata(noxSamples)
    NOX_Line.set_xdata(vocTimeSamples)

    voc_ax.set_xlim (min(vocTimeSamples) - 0.01, max(vocTimeSamples) + 0.01)
    nox_ax.set_xlim(min(vocTimeSamples) - 0.01, max(vocTimeSamples) + 0.01)
    voc_ax.set_ylim (min(vocSamples) - 200, max(vocSamples) + 200)
    nox_ax.set_ylim(min(noxSamples) - 300, max(noxSamples) + 200)


    #===========================================
    #+++++++++++++++ [1x1] +++++++++++++++++++++
    vocJumpSamples.append(VOC_jump)
    noxJumpSamples.append(NOX_jump)
    movJumpSamples.append(Movement)

    vocJumpSamples = vocJumpSamples[-voc_x_num_of_samples:]
    noxJumpSamples = noxJumpSamples[-voc_x_num_of_samples:]
    movJumpSamples = movJumpSamples[-ecg_acc_x_num_of_samples:]

    VOC_jump_Line.set_ydata(vocJumpSamples)
    VOC_jump_Line.set_xdata(vocTimeSamples)
    NOX_jump_Line.set_ydata(noxJumpSamples)
    NOX_jump_Line.set_xdata(vocTimeSamples)
    mov_jump_Line.set_ydata(movJumpSamples)
    mov_jump_Line.set_xdata(ecgAccTimeSamples)

    jump_ax.set_xlim (min(vocTimeSamples) - 0.01, max(vocTimeSamples) + 0.01)
    jump_ax.set_ylim (-1, 2)

    #===========================================
    #===========================================


    data_file.write(f"{th_timestamp}, {Temp}, {Humid},,")
    data_file.write(f"{PPG_timestamp}, {HR_val}, {SpO2_val},,")
    data_file.write(f"{VOC_timestamp}, {VOC_val}, {NOX_val}, {VOC_jump}, {NOX_jump},,")
    data_file.write(f"{ECG_ACC_timestamp}, {ECG_heart_rate}, {X_acc}, {Y_acc}, {Z_acc}, {Movement}\n")

    return Temp_Line, Humid_Line, PPG_HR_Line, SpO2_Line, ECG_HR_Line, VOC_Line, NOX_Line, VOC_jump_Line, mov_jump_Line

def record_and_plot ():

    data_file = open("Data/multinodal_readout.csv", "w")

    data_file.write("TH Timstamp, Temperature, Humidity,,")
    data_file.write("PPG Timestamp, HR, SpO2,,")
    data_file.write("VOC Timestamp, VOC, NOX, VOC_jump, NOX_jump,,")
    data_file.write("ECG_ACC Timestamp, HR, X_acc, Y_acc, Z_acc, Movement\n")

    # ------ Initialize Plot ------
    fig, ((temp_ax, voc_ax), (hr_ax, jump_ax)) = plt.subplots(2, 2, figsize=(12, 7))
    fig.suptitle("Sensor Data Readout")


    # ---- Temperature and Humidity ----
    Temp_Line, = temp_ax.plot(tempTimeSamples, tempSamples, color='blue', label="Temperature")
    humid_ax = temp_ax.twinx()
    Humid_Line, = humid_ax.plot(tempTimeSamples, humidSamples, color='red', label="Humidity")

    temp_ax.set_title("Temperature and Humidity")
    temp_ax.set_xlabel("Time (s)")
    temp_ax.set_ylabel("Temperature (C)")
    humid_ax.set_ylabel("Humidity (%)")

    plt.legend([Temp_Line, Humid_Line], ['Temperature', 'Humidity'])

    # ---- HR and SpO2 ----
    PPG_HR_Line, = hr_ax.plot(ppgTimeSamples, ppgHrSamples, color='blue', label="PPG HR")
    spo2_ax = hr_ax.twinx()
    SpO2_Line, = spo2_ax.plot(ppgTimeSamples, ppgSpO2Samples, color='red', label="SpO2")
    ECG_HR_Line, = hr_ax.plot(ecgAccTimeSamples, ecgHrSamples, color='green', label="ECG HR")

    hr_ax.set_title("Hear Rate and Oxygen Level (SpO2)")
    hr_ax.set_xlabel("Time (s)")
    hr_ax.set_ylabel("Heart Rate (bpm)")
    spo2_ax.set_ylabel("Oxygen Level (%)")
    plt.legend([PPG_HR_Line, SpO2_Line, ECG_HR_Line], ['PPG HR', 'SpO2', 'ECG HR'])

    # ---- VOC and NOX ----
    VOC_Line, = voc_ax.plot(vocTimeSamples, vocSamples, color='blue', label="VOC")
    nox_ax = voc_ax.twinx()
    NOX_Line, = nox_ax.plot(vocTimeSamples, noxSamples, color='red', label="NOX")

    voc_ax.set_title("VOC and NOX Raw Levels")
    voc_ax.set_xlabel("Time (s)")
    voc_ax.set_ylabel("VOC (raw)")
    nox_ax.set_ylabel("NOX (raw)")

    plt.legend([VOC_Line, NOX_Line], ['VOC', 'NOX'])

    # ---- VOC and NOX Jumps ----
    VOC_jump_Line, = jump_ax.plot(vocTimeSamples, vocJumpSamples, color='blue', label="VOC Jump")
    NOX_jump_Line, = jump_ax.plot(vocTimeSamples, noxJumpSamples, color='red', label="NOX Jump")
    mov_jump_Line, = jump_ax.plot(ecgAccTimeSamples, movJumpSamples, color='green', label="Movement")

    jump_ax.set_title("VOC and NOX Jumps and Detected Movement")
    jump_ax.set_xlabel("Time (s)")
    jump_ax.set_ylabel("Jump (True/False)")

    plt.legend([VOC_jump_Line, NOX_jump_Line, mov_jump_Line], ['VOC Jump', 'NOX Jump', 'Movement'])


    ani = animation.FuncAnimation(fig,
        animate,
        fargs=(data_file, Temp_Line, Humid_Line, temp_ax, humid_ax,
                PPG_HR_Line, SpO2_Line, ECG_HR_Line, hr_ax, spo2_ax,
                VOC_Line, NOX_Line, voc_ax, nox_ax,
                VOC_jump_Line, NOX_jump_Line, mov_jump_Line, jump_ax,
               ),
        interval=200,
        blit=False)
    plt.show()

    # data_file.close()



# =========================================================================
# =========================================================================
# =========================================================================
# =========================================================================



if __name__ == "__main__":
    # start_ploting()
    # th_device  = asyncio.run(BLEconnect(THName))
    # ppg_device = asyncio.run(BLEconnect(PPGName))
    # voc_device = asyncio.run(BLEconnect(VOCName))
    ecg_acc_device = asyncio.run(BLEconnect(ECG_ACCName))

    # init_ble_threads(devices=[th_device, ppg_device, voc_device, ecg_acc_device])
    test_thread = threading.Thread(target=async_entry_ecg_acc, args=(ecg_acc_device,))
    test_thread.start()

    record_and_plot()
    
