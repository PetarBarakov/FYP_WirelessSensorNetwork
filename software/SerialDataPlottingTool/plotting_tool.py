import pandas as pd
import matplotlib.pyplot as plt

import pandas as pd

from matplotlib.ticker import FormatStrFormatter

def plot_acc(file_path):
    # Read the CSV file into a DataFrame
    df = pd.read_csv(file_path)
    
    # Extract each column into a list
    time = df.iloc[:, 0].tolist()
    x = df.iloc[:, 1].tolist()
    y = df.iloc[:, 2].tolist()
    z = df.iloc[:, 3].tolist()
    flag = df.iloc[:, 4].tolist()

    x = [9.81*2*x_i for x_i in x]
    y = [9.81*2*y_i for y_i in y]
    z = [9.81*2*z_i for z_i in z]

    # Plot the data
    fig, ax = plt.subplots(4, 1, figsize=(6, 6))
    fig.suptitle('Accelerometer Data')
    ax[0].plot(time, x, label='X', color='blue')
    ax[1].plot(time, y, label='Y', color='green')
    ax[2].plot(time, z, label='Z', color='red')
    ax[3].plot(time, flag, label='Movement', color='orange')

    # ax[0].set_title('X Acceleration')
    # ax[1].set_title('Y Acceleration')
    # ax[2].set_title('Z Acceleration')
    # ax[3].set_title('Movement')

    ax[3].set_xlabel('Time (s)')

    # for i in range(4):
    #     ax[i].set_ylabel('Acceleration (m/s^2)')

    ax[0].set_ylabel('X ($m/s^2$)')
    ax[1].set_ylabel('Y ($m/s^2$)')
    ax[2].set_ylabel('Z ($m/s^2$)')
    ax[3].set_ylabel('Movement (0/1)')

    plt.show()
    

def plot_ecg(file_path):
    # Read the CSV file into a DataFrame
    df = pd.read_csv(file_path)
    
    # Extract each column into a list
    time = df.iloc[:, 0].tolist()
    hr = df.iloc[:, 1].tolist()
    raw = df.iloc[:, 2].tolist()
    filter = df.iloc[:, 3].tolist()

    # Plot the data
    fig, ax = plt.subplots(3, 1, figsize=(6, 5))
    fig.suptitle('ECG Data')
    ax[0].plot(time, raw, label='Raw ECG', color='blue')
    ax[1].plot(time, filter, label='Filtered ECG', color='green')
    ax[2].plot(time, hr, label='Heart Rate', color='red')
    ax[2].set_ylim(30, 80)

    ax[2].set_xlabel('Time (s)')

    ax[0].set_ylabel('Raw ECG')
    ax[1].set_ylabel('Filtered ECG')
    ax[2].set_ylabel('Heart Rate (bpm)')

    plt.show()
    

def plot_power(file_path):
    # Read the CSV file into a DataFrame
    df = pd.read_csv(file_path)
    
    # Extract each column into a list
    time = df.iloc[:, 0].tolist()
    I_in = df.iloc[:, 1].tolist()
    V_in = df.iloc[:, 2].tolist()
    I_esp = df.iloc[:, 3].tolist()
    V_esp = df.iloc[:, 4].tolist()
    I_sens = df.iloc[:, 5].tolist()
    V_sens = df.iloc[:, 6].tolist()
    I_ppg = df.iloc[:, 7].tolist()
    V_ppg = df.iloc[:, 8].tolist()

    P_in = [I_in[i]*V_in[i] for i in range(len(I_in))]
    P_esp = [I_esp[i]*V_esp[i] for i in range(len(I_esp))]
    P_sens = [I_sens[i]*V_sens[i] for i in range(len(I_sens))]
    P_ppg = [I_ppg[i]*V_ppg[i] for i in range(len(I_ppg))]

    # Plot the data
    fig, ax = plt.subplots(4, 1, figsize=(6, 5))
    fig.suptitle('ECG Data')
    ax[0].plot(time, I_in, label='Raw ECG', color='blue')
    ax[1].plot(time, I_esp, label='Filtered ECG', color='green')
    ax[2].plot(time, I_sens, label='Heart Rate', color='red')
    ax[3].plot(time, I_ppg, label='Heart Rate', color='red')

    # ax[2].set_ylim(30, 80)
    ax[2].set_xlabel('Time (s)')
    
    # ax[0].set_xlim(2550, 2555)
    # ax[1].set_xlim(2550, 2555)
    # ax[2].set_xlim(2550, 2555)
    # ax[3].set_xlim(2550, 2555)

    # ax[0].set_ylabel('Raw ECG')
    # ax[1].set_ylabel('Filtered ECG')
    # ax[2].set_ylabel('Heart Rate (bpm)')
    
    average_in_power = sum(P_in)/len(P_in)
    average_esp_power = sum(P_esp)/len(P_esp)
    average_sens_power = sum(P_sens)/len(P_sens)
    average_ppg_power = sum(P_ppg)/len(P_ppg)

    average_in_current = sum(I_in)/len(I_in)
    average_esp_current = sum(I_esp)/len(I_esp)
    average_sens_current = sum(I_sens)/len(I_sens)
    average_ppg_current = sum(I_ppg)/len(I_ppg)

    average_in_voltage = sum(V_in)/len(V_in)
    average_esp_voltage = sum(V_esp)/len(V_esp)
    average_sens_voltage = sum(V_sens)/len(V_sens)
    average_ppg_voltage = sum(V_ppg)/len(V_ppg)

    print(f'Average input power: {average_in_power} W, Average input current: {average_in_current} A, Average input voltage: {average_in_voltage} V')
    print(f'Average esp power: {average_esp_power} W, Average esp current: {average_esp_current} A, Average esp voltage: {average_esp_voltage} V')
    print(f'Average sens power: {average_sens_power} W, Average sens current: {average_sens_current} A, Average sens voltage: {average_sens_voltage} V')
    print(f'Average ppg power: {average_ppg_power} W, Average ppg current: {average_ppg_current} A, Average ppg voltage: {average_ppg_voltage} V')


    plt.show()

def plot_ppg(file_path):
    # Read the CSV file into a DataFrame
    df = pd.read_csv(file_path)
    
    # Extract each column into a list
    time = df.iloc[:, 0].tolist()
    red = df.iloc[:, 1].tolist()
    ir = df.iloc[:, 2].tolist()

    red = [red_i/1e6 for red_i in red]
    ir = [ir_i/1e6 for ir_i in ir]

    # Plot the data
    fig, ax = plt.subplots(2, 1, figsize=(9, 6))
    fig.suptitle('PPG Raw Data Readout')
    ax[0].plot(time, red, label='Raw Red', color='blue')
    ax[1].plot(time, ir, label='Raw IR', color='red')

    ax[1].set_xlabel('Time (s)')

    ax[0].set_ylabel('Raw Red ($x10^6$)')
    ax[1].set_ylabel('Raw IR ($x10^6$)')

    # ax[0].set_ylim([2.2610, 2.2614])

    # ax[0].ticklabel_format(style='plain')
    ax[0].yaxis.set_major_formatter(FormatStrFormatter('%.4f'))
    ax[0].set_xlim(15, 20)   
    ax[1].set_xlim(15, 20)   

    plt.show()

# Example usage
if __name__ == "__main__":
    # acc_path = 'SerialData/acc_data1_selected.csv'
    # plot_acc(acc_path)

    # ecg_path = 'SerialData/ecg_data2_selected.csv'
    # plot_ecg(ecg_path)

    # ppg_path = 'SerialData/ppg_data4.csv'
    # plot_ppg(ppg_path)


    power_path = 'SerialData/power_measurements_ble_en.csv'
    plot_power(power_path)