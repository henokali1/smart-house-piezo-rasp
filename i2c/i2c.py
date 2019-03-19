#!/usr/bin/python

import smbus
import time

bus = smbus.SMBus(1)
#address is address of the register(the I2C decvice)
def readArdu(address):
    msg = ""
    decoded = []
    a = bus.read_i2c_block_data(address, 1)[0:255]
    for i in range(len(a)):
        if(chr(a[i]) != '\xff'):
            msg = msg + chr(a[i])
            decoded.append(chr(a[i]))
    return ''.join(decoded)

   
responce = (readArdu(8))
responceList = responce.split(',')
print(responce)

