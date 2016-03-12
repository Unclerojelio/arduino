from __future__ import division
'''
Created on Jun 10, 2011

@author: bitrex@earthlink.net
'''
'''
    LEDAnalyzer script which, when combined with Arduino and support hardware,
    will create a plot of the LED under test's I-V characteristics and fit
    a curve to the generated data.

    Copyright (C) 2011 bitrex@earthlink.net

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
'''

import serial
import sys
import getopt
import time
import numpy
import matplotlib.pyplot as plt
import scipy as sp
import scipy.optimize

def main(argv):
    
    port = "COM2"
    baud = 9600
    iterations = 1

    try: 
        opts, args = getopt.getopt(argv, "hp:b:i:",["port=","baudrate=","iterations="])
    
    except getopt.GetoptError:
        usage()
        sys.exit(2)
    
    for opt, arg in opts:
        if opt in ["--help","-h"]:    
            usage()
            sys.exit()
        if opt in ["--port","-p"]:
            port = arg
        if opt in ["--baud","-b"]:
            baud = int(arg)
        if opt in ["--iterations","-i"]:
            iterations = int(arg)

    Analyzer(port, baud, iterations)
    
def usage():
    
    sys.stdout.write("-p PORT  -COM port for Arduino USB to serial converter (default COM2)\n")
    sys.stdout.write("-b BAUD  -Baud rate Arduino analyzer sketch is set for (default 9600)\n")
    sys.stdout.write("-i ITERATIONS  -Number of iterations to run analyzer for averaging purposes\n(default 1)\n")
    sys.stdout.write("-h             -This help text\n")

def Analyzer(comPort, baud, iterations):
    
    timeout = 5
    startByte = endByte = '!'
    senseResistor = 100 #ohms
    voltageRange = 3 #volts 
    currentDataList = []
    voltageDataList = [] 
     
    for iteration in xrange(0,iterations): 
   
        receivedData1 = []
        receivedData2 = []
        voltageValues = []
        currentValues = []
        dataCount = 0    
    
    # Sends test start command to Arduino and receives data
        
        try:
            ser = serial.Serial(comPort, baud, timeout=5, writeTimeout=5)
            if baud not in ser.BAUDRATES:
                sys.stderr.write("Baud rate %i not supported by COM port!" % baud)
                sys.exit(1)
        
        except serial.SerialException:
            sys.stderr.write("Unable to open serial port %s" % comPort)
            sys.exit(1)
        
        sys.stdout.write("Waiting for reset...\n")
        time.sleep(5) #Wait for Ardunio to reset after serial port is opened
    
        try:
            startTime = time.time()
            ser.write(startByte)
            ackData = None
            
            while True:
                try:
                    if ser.inWaiting() > 0:
                        try:
                            ackData = ser.read()
                        except serial.SerialException:
                            sys.stderr.write("Error reading data from serial port %s" % comPort)
                            ser.close()
                            sys.exit(1)
                    if ackData != startByte and (time.time() - startTime) > timeout:
                        raise serial.SerialTimeoutException
                    elif ackData == startByte:
                        break

                except serial.SerialException:
                    raise serial.SerialTimeoutException
                
        except serial.SerialTimeoutException:
            sys.stderr.write("Error writing to serial port %s" % comPort)
            ser.close()
            sys.exit(1)
        
        sys.stdout.write("Running analyzer...")
        transmitCount = 0
        currentTime = time.time()

        while True:
            try:  
                if ser.inWaiting() > 0:        
                    try:       
                        data = ser.read()
                        transmitCount += 1
                        currentTime = time.time()
                        if divmod(transmitCount,100)[1] == 0:    
                            sys.stdout.write('.')
                        if transmitCount > 2000 and data == endByte:  # start looking for the end byte near the end of the data                  
                            ser.close()
                            break
                        else:
                            if dataCount == 0:    
                                receivedData1.append(ord(data))            
                            else:
                                receivedData2.append(ord(data))
                        if dataCount == 0:
                            dataCount += 1
                        else:
                            dataCount = 0    
                
                    except serial.SerialException:
                        raise serial.SerialException
                
                if (time.time() - currentTime) > timeout:
                    raise serial.SerialTimeoutException
                        
            except serial.SerialException, serial.SerialTimeoutException:
                sys.stderr.write("Error reading data from serial port %s" % comPort)
                ser.close()
                sys.exit(1)  
        
        for index, data in enumerate(receivedData1): 
                data = data << 2 #shift MSB of data back to its original position
                voltageValues.append(data+receivedData2[index]) # 10 bit result
        
        voltageValues = map(lambda x: (x/1024)*5, voltageValues) #convert DAC values to voltage levels based on 5 volt DAC reference        
        
        voltageValues[0] = 0  # 0 voltage at 0 current
        
        currentValues = [((elem/((4095/5)*voltageRange))*voltageRange)/senseResistor 
                                   for elem in range(0, int((4095/5)*voltageRange))] #generate the values of current for the Y axis
        
        voltageDataList.append(voltageValues)
        currentDataList.append(currentValues)
        sys.stdout.write("\nIteration # %i complete.\n" % (iteration+1))
    
    sys.stdout.write("\nAnalyzer complete.") 
    createPlot(currentDataList, voltageDataList)

def createPlot(currentDataList, voltageDataList):
    
    currentAverageArray = []
    voltageAverageArray = []
    
    for index in xrange(0, len(currentDataList[0])):  # these fucntions create an average value over the number of iterations run
        tempAverage = []
        for element in currentDataList:
            tempAverage.append(element[index])
        currentAverageArray.append(numpy.average(tempAverage))
    currentAverageArray = numpy.array(currentAverageArray)
    
    for index in xrange(0, len(voltageDataList[0])):
        tempAverage = []
        for element in voltageDataList:
            tempAverage.append(element[index])
        voltageAverageArray.append(numpy.average(tempAverage))
    
    voltageAverageArray = numpy.array(voltageAverageArray)    
    
    A,B,C = fit_exp(currentAverageArray, voltageAverageArray)
    fit_V = numpy.linspace(0,5, 1000)
    fit_I = fit_function(fit_V,A,B,C)
    
    plot1 = plt.semilogx(currentAverageArray,voltageAverageArray,'r+')
    plot2 = plt.semilogx(fit_I,fit_V)
    axis1 = plt.axis([0,0.04,0,5])
    xl = plt.xlabel('LED Current')
    yl = plt.ylabel('LED Voltage')
    
    plt.legend((plot1,plot2),['LED Forward Current Measured Characteristics\n','Exponential Fit:\n $ I = %0.3g e^{%0.2fV} $' % (A,B)])
    plt.show()

def fit_function(V,A,B,C):
    return A*numpy.exp(V*B) + C

def fit_exp(I, V):
    
    try:
        p0 = [10e-8,1,0] #initial guesses
        opt_parms, parm_cov = sp.optimize.curve_fit(fit_function,V,I,p0,maxfev=10000)
        A, B, C = opt_parms        
        return A, B, C
        
    except RuntimeError:
        sys.stderr.write("Unable to fit a curve to analyzer data.")
        sys.exit(1)
                
if __name__ == '__main__':
    
    main(sys.argv[1:])