# coding=utf-8
import serial
import time
import json
import csv
import os
import serial.tools.list_ports


def main():

    time.sleep(5)
    ser.write("begin".encode('UTF-8'))

    while True:
        read_data()
    pass


# Get lora message!
# [SX1278] Data:			
# {"ID":"0xAAAA","SLEEP":10,"temp":0.00,"humi":0.00,"eco2":0.00,"lux":0.00}
# [SX1278] RSSI:			-55 dBm
# [SX1278] SNR:			9.75 dB
# [SX1278] Frequency error:	4744.54 Hz
# Get lora message!
# [SX1278] Data:			
# {"ID":"0xBBBB","SLEEP":10,"temp":0.00,"humi":0.00,"eco2":0.00,"lux":0.00}
# [SX1278] RSSI:			-41 dBm
# [SX1278] SNR:			9.50 dB
# [SX1278] Frequency error:	767.82 Hz
# Get lora message!
# [SX1278] Data:			
# {"ID":"0xAAAA","SLEEP":10,"temp":0.00,"humi":0.00,"eco2":0.00,"lux":0.00}
# [SX1278] RSSI:			-54 dBm
# [SX1278] SNR:			9.50 dB
# [SX1278] Frequency error:	4744.02 Hz


def read_data():

    ser.reset_input_buffer()
    line = ser.readline().decode('UTF-8').replace('\n', '')
    # line = ser.readline().decode('UTF-8')
    # print(line)

    try:
        data = json.loads(line)
        list_title = []
        list_value = []
        print(data)
        for key in data:
            # print(key, data[key])
            list_title.append(key)
            list_value.append(data[key])

        time_str = time.strftime("%Y-%m-%d %H:%M:%S ",
                                 time.localtime(time.time()))
        print(time_str)
        print(list_title)
        print(list_value)

        list_value.append(time_str)
        list_title.append("Time")

        if list_title[0] == "ID":
            save_data(list_title, list_value)

    except:
        print("LOG:", line)


def save_data(list_title, list_value):
    filename = str(list_value[0]) + "_data.csv"
    title_flag = 0

    # 判断文件是否存在
    if os.path.exists(filename):
        print('Old sensor')

    else:
        print('New sensor')
        title_flag = 1

    file = open(filename, mode='a', newline='')
    writer = csv.writer(file)
    if title_flag == 1:
        writer.writerow(list_title)
    writer.writerow(list_value)

    file.close()


def get_com_list():
    global port_list
    port_list = serial.tools.list_ports.comports()
    return port_list


def get_frist_com():
    if len(get_com_list()) <= 0:
        print("No COM")
        return ""
    else:
        print("First COM")
        port_list = get_com_list()
        for com in port_list:
            print(com)
            for i in com:
                print(i)
                if i == "RaspberryPi Pico":
                    return list(com)[0]
            
        return NULL

# print("Input Serial number, don't add COM, just number.")
# serial_num = input()
# serial_num = "COM" + str(serial_num)
# ser = serial.Serial(serial_num, 115200)


ser = serial.Serial(get_frist_com(), 115200)

if __name__ == '__main__':
    main()
