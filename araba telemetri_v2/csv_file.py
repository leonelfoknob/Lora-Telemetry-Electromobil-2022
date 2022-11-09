import csv
import serial
import time

file_name = 'data_loger.csv'
with open(file_name,'w+') as csvFile:
	csvFile.write('hiz,gerilim,sicaklik,şarj_durumu,akim\n')

#read data from arduino python cod


ser = serial.Serial('/dev/ttyACM0',9600, timeout=0.1)
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
        gerilim = rows[1]
        sicaklik= rows[2]
        sarj_durum = rows[3]
        akim = rows[4]
        with open(file_name,'a+') as csvFile:
        	csvFile.write(hiz + ',' + gerilim+ ',' +sicaklik+ ',' +sarj_durum+ ',' +akim+ '\n')
        '''Myfile = open('tel_file.csv','a+')
        #Myfile.write('Temperature Sensor  Value : ')
        Myfile.write('\n'+'hiz:'+ str(hiz) + 'gerilim :'+ str(gerilim ) + 'sicaklik :'+ str(sicaklik) +'sarj_durum :'+ str(sarj_durum) +'akim :'+ str(akim))
        Myfile.close()'''

        print(value)
        print(rows)
        print("hiz : " +  str(hiz))
        print("gerilim  : " +  str(gerilim))
        print("sicaklik: " +  str(sicaklik))
        print("ReadSealevelPressure : " +  str(sarj_durum))
        print("akim : " +  str(akim))

