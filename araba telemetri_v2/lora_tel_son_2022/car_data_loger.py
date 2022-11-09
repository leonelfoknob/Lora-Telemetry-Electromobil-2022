import csv
import serial
import time

file_name = 'car_data_loger.csv'
with open(file_name,'w+') as csvFile:
	csvFile.write('hiz,max_temp,volts,currents,power,energy\n')

#read data from arduino python cod


ser = serial.Serial('/dev/ttyUSB0',9600, timeout=0.1)
#ser.close()
#ser.open()
#Myfile = open('tel_file.csv','w')

#arduino = serial.Serial('/dev/ttyUSB0',9600, timeout=0.1)

def read_sensor_data():
    data = ser.readline()
    return data
    #return data.decode('utf-8')#convert reciving data by in string

while True:
    #value = read_sensor_data
    #time.sleep(1)
    data = ser.readline()
    if data:
        value = data.decode('utf-8')
        rows = [x for x in value.split(',')]
        hiz = rows[0] 
        max_temp= rows[1]
        volts= rows[2]
        currents= rows[3]
        power= rows[4]
        energy= rows[5]
        with open(file_name,'a+') as csvFile:
        	csvFile.write(hiz + ',' + max_temp+ ',' +volts+ ',' +currents+ ',' +power+ ',' +energy+ '\n')
        print(value)
        print(rows)
        print("hiz : " +  str(hiz))
        print("max_temp : " +  str(max_temp))
        print("volts: " +  str(volts))
        print("Rcurrents: " +  str(currents))
        print("power : " +  str(power))
        print("energy : " +  str(energy))

