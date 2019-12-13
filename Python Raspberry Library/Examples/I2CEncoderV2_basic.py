import smbus2
import RPi.GPIO as GPIO
from time import sleep
import i2cEncoderLibV2


def EncoderChange():
    encoder.writeLEDG(100)
    print ('Changed: %d' % (encoder.readCounter32()))
    encoder.writeLEDG(0)

def EncoderPush():
    encoder.writeLEDB(100)
    print ('Encoder Pushed!')
    encoder.writeLEDB(0)

def EncoderDoublePush():
    encoder.writeLEDB(100)
    encoder.writeLEDG(100)
    print ('Encoder Double Push!')
    encoder.writeLEDB(0)
    encoder.writeLEDG(0)

def EncoderMax():
    encoder.writeLEDR(100)
    print ('Encoder max!')
    encoder.writeLEDR(0)

def EncoderMin():
    encoder.writeLEDR(100)
    print ('Encoder min!')
    encoder.writeLEDR(0)

def Encoder_INT(self):
    encoder.updateStatus()


GPIO.setmode(GPIO.BCM)
bus = smbus2.SMBus(1)
INT_pin = 4
GPIO.setup(INT_pin, GPIO.IN, pull_up_down=GPIO.PUD_UP)

encoder = i2cEncoderLibV2.i2cEncoderLibV2(bus, 0x40)

encconfig = (i2cEncoderLibV2.INT_DATA | i2cEncoderLibV2.WRAP_ENABLE | i2cEncoderLibV2.DIRE_RIGHT | i2cEncoderLibV2.IPUP_ENABLE | i2cEncoderLibV2.RMOD_X1 | i2cEncoderLibV2.RGB_ENCODER)
encoder.begin(encconfig)

encoder.writeCounter(0)
encoder.writeMax(35)
encoder.writeMin(-20)
encoder.writeStep(1)
encoder.writeAntibouncingPeriod(8)
encoder.writeDoublePushPeriod(50)
encoder.writeGammaRLED(i2cEncoderLibV2.GAMMA_2)
encoder.writeGammaGLED(i2cEncoderLibV2.GAMMA_2)
encoder.writeGammaBLED(i2cEncoderLibV2.GAMMA_2)

encoder.onChange = EncoderChange
encoder.onButtonPush = EncoderPush
encoder.onButtonDoublePush = EncoderDoublePush
encoder.onMax = EncoderMax
encoder.onMin = EncoderMin

encoder.autoconfigInterrupt()
print ('Board ID code: 0x%X' % (encoder.readIDCode()))
print ('Board Version: 0x%X' % (encoder.readVersion()))

encoder.writeRGBCode(0x640000)
sleep(0.3)
encoder.writeRGBCode(0x006400)
sleep(0.3)
encoder.writeRGBCode(0x000064)
sleep(0.3)
encoder.writeRGBCode(0x00)

GPIO.add_event_detect(INT_pin, GPIO.FALLING, callback=Encoder_INT, bouncetime=10)

while True:
  #  if GPIO.input(INT_pin) == False: #
   #     Encoder_INT() #
    pass
