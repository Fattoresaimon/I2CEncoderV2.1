import smbus2
##import smbus 
from gpiozero import Button 
from time import sleep
import i2cEncoderLibV2

bus=smbus2.SMBus(1)
##bus=smbus.SMBus(1)
int_pin = Button(4)


encoder = i2cEncoderLibV2.i2cEncoderLibV2(bus,0x61)


encconfig=(i2cEncoderLibV2.INT_DATA | i2cEncoderLibV2.WRAP_ENABLE | i2cEncoderLibV2.DIRE_RIGHT | i2cEncoderLibV2.IPUP_ENABLE | i2cEncoderLibV2.RMOD_X1 | i2cEncoderLibV2.RGB_ENCODER)
encoder.begin(encconfig)


encoder.writeCounter(0)
encoder.writeMax(100.0)
encoder.writeMin(0)
encoder.writeStep(2)
encoder.writeInterruptConfig(0xff)

encoder.writeRGBCode(0xFF0000)
sleep(0.3)
encoder.writeRGBCode(0x00FF00)
sleep(0.3)
encoder.writeRGBCode(0x0000FF)
sleep(0.3)
encoder.writeRGBCode(0x00)


while True:
	if int_pin.is_pressed :
		encoder.updateStatus()
		if encoder.readStatus(i2cEncoderLibV2.RINC) == True :
			print ('Increment: %d' % (encoder.readCounter32())) 
		elif encoder.readStatus(i2cEncoderLibV2.RDEC) == True :
			print ('Decrement:  %d' % (encoder.readCounter32())) 
		if encoder.readStatus(i2cEncoderLibV2.RMAX) == True :
			print ('Max!') 
		elif encoder.readStatus(i2cEncoderLibV2.RMIN) == True :
			print ('Min!') 