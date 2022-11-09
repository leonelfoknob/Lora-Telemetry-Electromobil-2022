#read data from arduino python cod
import serial
import time

ser = serial.Serial('/dev/ttyUSB1',9600, timeout=0.1)
ser.close()
ser.open()
Myfile = open('tel_file.txt','w')
#arduino = serial.Serial('/dev/ttyUSB0',9600, timeout=0.1)

def read_sensor_data():
    data = ser.readline()
    return data
    #return data.decode('utf-8')#convert reciving data by in string

while True:
    #value = read_sensor_data
    sleep(5)
    data = ser.readline()
    if data:
        value = data.decode('utf-8')
        rows = [x for x in value.split(',')]
        temperature = rows[0] 
        pressure = rows[1]
        altitude = rows[2]
        ReadSealevelPressure = rows[3]
        Real_altitude = rows[4]
        Myfile = open('tel_file.txt','a+')
        #Myfile.write('Temperature Sensor  Value : ')
        Myfile.write('\n'+'Data:'+ str(temperature))
        Myfile.close()

        print(value) #affiche les valeur brute
        print(rows) #affiche les valeur enregistrer dans la liste
        print("trmperature : " +  str(temperature)) #affiche la valeur temperature enregistrer comme 1er element de la liste
        print("Pressure : " +  str(pressure))
        print("Altitude : " +  str(altitude))
        print("ReadSealevelPressure : " +  str(ReadSealevelPressure))
        print("Real_altitude : " +  str(Real_altitude))

