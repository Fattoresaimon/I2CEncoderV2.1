import smbus
import time
import struct
from time import sleep

# Encoder register definition#
REG_GCONF = 0x00
REG_GP1CONF = 0x01
REG_GP2CONF = 0x02
REG_GP3CONF = 0x03
REG_INTCONF = 0x04
REG_ESTATUS = 0x05
REG_I2STATUS = 0x06
REG_FSTATUS = 0x07
REG_CVALB4 = 0x08
REG_CVALB3 = 0x09
REG_CVALB2 = 0x0A
REG_CVALB1 = 0x0B
REG_CMAXB4 = 0x0C
REG_CMAXB3 = 0x0D
REG_CMAXB2 = 0x0E
REG_CMAXB1 = 0x0F
REG_CMINB4 = 0x10
REG_CMINB3 = 0x11
REG_CMINB2 = 0x12
REG_CMINB1 = 0x13
REG_ISTEPB4 = 0x14
REG_ISTEPB3 = 0x15
REG_ISTEPB2 = 0x16
REG_ISTEPB1 = 0x17
REG_RLED = 0x18
REG_GLED = 0x19
REG_BLED = 0x1A
REG_GP1REG = 0x1B
REG_GP2REG = 0x1C
REG_GP3REG = 0x1D
REG_ANTBOUNC = 0x1E
REG_DPPERIOD = 0x1F
REG_FADERGB = 0x20
REG_FADEGP = 0x21
REG_GAMRLED = 0x27
REG_GAMGLED = 0x28
REG_GAMBLED = 0x29
REG_GAMMAGP1 = 0x2A
REG_GAMMAGP2 = 0x2B
REG_GAMMAGP3 = 0x2C
REG_GCONF2 = 0x30
REG_IDCODE = 0x70
REG_VERSION = 0x71
REG_EEPROMS = 0x80

# Encoder configuration bit. Use with GCONF #
FLOAT_DATA = 0x0001
INT_DATA = 0x0000
WRAP_ENABLE = 0x0002
WRAP_DISABLE = 0x0000
DIRE_LEFT = 0x0004
DIRE_RIGHT = 0x0000
IPUP_DISABLE = 0x0008
IPUP_ENABLE = 0x0000
RMOD_X2 = 0x0010
RMOD_X1 = 0x0000
RGB_ENCODER = 0x0020
STD_ENCODER = 0x0000
EEPROM_BANK1 = 0x0040
EEPROM_BANK2 = 0x0000
RESET = 0x0080
CLK_STRECH_ENABLE = 0x0100
CLK_STRECH_DISABLE = 0x0000
REL_MODE_ENABLE = 0x0200
REL_MODE_DISABLE = 0x0000

# Encoder status bits and setting. Use with: INTCONF for set and with ESTATUS for read the bits  #
PUSHR = 0x01
PUSHP = 0x02
PUSHD = 0x04
RINC = 0x08
RDEC = 0x10
RMAX = 0x20
RMIN = 0x40
INT_2 = 0x80

# Encoder Int2 bits. Use to read the bits of I2STATUS  #
GP1_POS = 0x01
GP1_NEG = 0x02
GP2_POS = 0x04
GP2_NEG = 0x08
GP3_POS = 0x10
GP3_NEG = 0x20
FADE_INT = 0x40

# Encoder Fade status bits. Use to read the bits of FSTATUS  #
FADE_R = 0x01
FADE_G = 0x02
FADE_B = 0x04
FADE_GP1 = 0x08
FADE_GP2 = 0x10
FADE_GP3 = 0x20

# GPIO Configuration. USe with GP1CONF,GP2CONF,GP3CONF #
GP_PWM = 0x00
GP_OUT = 0x01
GP_AN = 0x02
GP_IN = 0x03

GP_PULL_EN = 0x04
GP_PULL_DI = 0x00

GP_INT_DI = 0x00
GP_INT_PE = 0x08
GP_INT_NE = 0x10
GP_INT_BE = 0x18

# Gamma configuration #

GAMMA_OFF = 0
GAMMA_1 = 1,
GAMMA_1_8 = 2
GAMMA_2 = 3
GAMMA_2_2 = 4
GAMMA_2_4 = 5
GAMMA_2_6 = 6
GAMMA_2_8 = 7


class i2cEncoderLibV2:

    onButtonRelease = None
    onButtonPush = None
    onButtonDoublePush = None
    onIncrement = None
    onDecrement = None
    onChange = None
    onMax = None
    onMin = None
    onMinMax = None
    onGP1Rise = None
    onGP1Fall = None
    onGP2Rise = None
    onGP2Fall = None
    onGP3Rise = None
    onGP3Fall = None
    onFadeProcess = None

    stat = 0
    stat2 = 0
    gconf = 0

# Class costructor

    def __init__(self, bus, add):
        self.i2cbus = bus
        self.i2cadd = add

# Used for initialize the encoder
    def begin(self, conf):
        self.writeEncoder8(REG_GCONF, conf & 0xFF)
        self.writeEncoder8(REG_GCONF2, ((conf >> 8) & 0xFF))
        self.gconf = conf

    def reset(self) :
        self.writeEncoder8(REG_GCONF, 0x80)

# Call che attached callaback if it is defined #
    def eventCaller(self, event) :
        if event:
            event()

# Return true if the status of the encoder changed, otherwise return false #
    def updateStatus(self) :
        self.stat = self.readEncoder8(REG_ESTATUS)
        
        if (self.stat == 0):
            self.stat2 = 0
            return False
        
        if (self.stat & PUSHR) != 0 :
            self.eventCaller (self.onButtonRelease)

        if (self.stat & PUSHP) != 0 :
            self.eventCaller (self.onButtonPush)

        if (self.stat & PUSHD) != 0 :
            self.eventCaller (self.onButtonDoublePush)

        if (self.stat & RINC) != 0 :
            self.eventCaller (self.onIncrement)
            self.eventCaller (self.onChange)

        if (self.stat & RDEC) != 0 :
            self.eventCaller (self.onDecrement)
            self.eventCaller (self.onChange)

        if (self.stat & RMAX) != 0 :
            self.eventCaller (self.onMax)
            self.eventCaller (self.onMinMax)

        if (self.stat & RMIN) != 0 :
            self.eventCaller (self.onMin)
            self.eventCaller (self.onMinMax)

        if (self.stat & INT_2) != 0 :
            self.stat2 = self.readEncoder8(REG_I2STATUS)
            if (self.stat2 == 0) :
                return True

            if (self.stat2 & GP1_POS) != 0 :
                self.eventCaller (self.onGP1Rise)

            if (self.stat2 & GP1_NEG) != 0 :
                self.eventCaller (self.onGP1Fall)

            if (self.stat2 & GP2_POS) != 0 :
                self.eventCaller (self.onGP2Rise)

            if (self.stat2 & GP2_NEG) != 0 :
                self.eventCaller (self.onGP2Fall)

            if (self.stat2 & GP3_POS) != 0 :
                self.eventCaller (self.onGP3Rise)

            if (self.stat2 & GP3_NEG) != 0 :
                self.eventCaller (self.onGP3Fall)

            if (self.stat2 & FADE_INT) != 0 :
                self.eventCaller (self.onFadeProcess)
        return True

#********************************* Read functions ***********************************#

# Return the GP1 Configuration#
    def readGP1conf(self) :
        return (self.readEncoder8(REG_GP1CONF))

# Return the GP1 Configuration#
    def readGP2conf(self) :
        return (self.readEncoder8(REG_GP2CONF))

# Return the GP1 Configuration#
    def readGP3conf(self) :
        return (self.readEncoder8(REG_GP3CONF))

# Return the INT pin configuration#
    def readInterruptConfig(self) :
        return (self.readEncoder8(REG_INTCONF))

# Check if a particular status match, return true is match otherwise false. Before require updateStatus() #
    def readStatus(self, status) :
        if (self.stat & status) != 0 :
            return True
        else:
            return False

# Return the status of the encoder #
    def readStatusRaw(self) :
        return self.stat

# Check if a particular status of the Int2 match, return true is match otherwise false. Before require updateStatus() #
    def readInt2(self, status) :
        if (self.stat2 & status) != 0 :
            return True
        else:
            return False

# Return the Int2 status of the encoder. Before require updateStatus()  #
    def readInt2Raw(self):
        return self.stat2

# Return Fade process status  #
    def readFadeStatusRaw(self):
        return self.readEncoder8(REG_FSTATUS)

# Check if a particular status of the Fade process match, return true is match otherwise false. #
    def readFadeStatus(self, status):
        if (self.readEncoder8(REG_FSTATUS) & status) == 1 :
            return True
        else:
            return False

# Return the PWM LED R value  #
    def readLEDR(self) :
        return (self.readEncoder8(REG_RLED))

# Return the PWM LED G value  #
    def readLEDG(self) :
        return (self.readEncoder8(REG_GLED))

# Return the PWM LED B value  #
    def readLEDB(self) :
        return (self.readEncoder8(REG_BLED))

# Return the 32 bit value of the encoder counter  #
    def readCounterFloat(self) :
        return (self.readEncoderFloat(REG_CVALB4))

# Return the 32 bit value of the encoder counter  #
    def readCounter32(self) :
        return (self.readEncoder32(REG_CVALB4))

# Return the 16 bit value of the encoder counter  #
    def readCounter16(self) :
        return (self.readEncoder16(REG_CVALB2))

# Return the 8 bit value of the encoder counter  #
    def readCounter8(self) :
        return (self.readEncoder8(REG_CVALB1))

# Return the Maximum threshold of the counter #
    def readMax(self) :
        return (self.readEncoder32(REG_CMAXB4))

# Return the Minimum threshold of the counter #
    def readMin(self) :
        return (self.readEncoder32(REG_CMINB4))

# Return the Maximum threshold of the counter #
    def readMaxFloat(self) :
        return (self.readEncoderFloat(REG_CMAXB4))

# Return the Minimum threshold of the counter #
    def readMinFloat(self) :
        return (self.readEncoderFloat(REG_CMINB4))

# Return the Steps increment #
    def readStep(self) :
        return (self.readEncoder16(REG_ISTEPB4))

# Return the Steps increment, in float variable #
    def readStepFloat(self) :
        return (self.readEncoderFloat(REG_ISTEPB4))

# Read GP1 register value #
    def readGP1(self) :
        return (self.readEncoder8(REG_GP1REG))

# Read GP2 register value #
    def readGP2(self) :
        return (self.readEncoder8(REG_GP2REG))

# Read GP3 register value #
    def readGP3(self) :
        return (self.readEncoder8(REG_GP3REG))

# Read Anti-bouncing period register #
    def readAntibouncingPeriod(self) :
        return (self.readEncoder8(REG_ANTBOUNC))

# Read Double push period register #
    def readDoublePushPeriod(self) :
        return (self.readEncoder8(REG_DPPERIOD))

# Read the fade period of the RGB LED#
    def readFadeRGB(self) :
        return (self.readEncoder8(REG_FADERGB))

# Read the fade period of the GP LED#
    def readFadeGP(self):
        return self.readEncoder8(REG_FADEGP)

# Read the ID code #
    def readIDCode(self):
        return self.readEncoder8(REG_IDCODE)

# Read the Version code #
    def readVersion(self):
        return self.readEncoder8(REG_VERSION)

# Read the EEPROM memory#
    def readEEPROM(self, add):

        if add <= 0x7f:
            if (self.gconf & EEPROM_BANK1) != 0:
                self.gconf = self.gconf & 0xBF
                self.writeEncoder8(REG_GCONF, self.gconf)

            data = self.readEncoder8((REG_EEPROMS + add))
        else:
            if (self.gconf & EEPROM_BANK1) == 0:
                self.gconf = self.gconf | 0x40
                self.writeEncoder8(REG_GCONF, self.gconf)

            data = self.readEncoder8(add)

        sleep(0.001)
        return (data)

#********************************* Write functions ***********************************#
# Write the GP1 configuration#
    def writeGP1conf(self, gp1) :
        self.writeEncoder8(REG_GP1CONF, gp1)

# Write the GP2 configuration#
    def writeGP2conf(self, gp2) :
        self.writeEncoder8(REG_GP2CONF, gp2)

# Write the GP3 configuration#
    def writeGP3conf(self, gp3) :
        self.writeEncoder8(REG_GP3CONF, gp3)

# Write the interrupt configuration #
    def writeInterruptConfig(self, interrupt) :
        self.writeEncoder8(REG_INTCONF, interrupt)

# Autoconfigure the interrupt register according to the callback declared #
    def autoconfigInterrupt(self) :
        reg = 0

        if (self.onButtonRelease != None):
            reg = reg | PUSHR

        if (self.onButtonPush != None):
            reg = reg | PUSHP

        if (self.onButtonDoublePush != None):
            reg = reg | PUSHD

        if (self.onIncrement != None):
            reg = reg | RINC

        if (self.onDecrement != None):
            reg = reg | RDEC

        if (self.onChange != None):
            reg = reg | RINC
            reg = reg | RDEC

        if (self.onMax != None):
            reg = reg | RMAX

        if (self.onMin != None):
            reg = reg | RMIN

        if (self.onMinMax != None): 
            reg = reg | RMAX
            reg = reg | RMIN

        if (self.onGP1Rise != None):
            reg = reg | INT_2

        if (self.onGP1Fall != None):
            reg = reg | INT_2

        if (self.onGP2Rise != None):
            reg = reg | INT_2

        if (self.onGP2Fall != None):
            reg = reg | INT_2

        if (self.onGP3Rise != None):
            reg = reg | INT_2

        if (self.onGP3Fall != None):
            reg = reg | INT_2

        if (self.onFadeProcess != None):
            reg = reg | INT_2

        self.writeEncoder8(REG_INTCONF, reg)

# Write the counter value #
    def writeCounter(self, value) :
        self.writeEncoder32(REG_CVALB4, value)

# Write the counter value #
    def writeCounterFloat(self, value) :
        self.writeEncoderFloat(REG_CVALB4, value)

# Write the maximum threshold value #
    def writeMax(self, max) :
        self.writeEncoder32(REG_CMAXB4, max)

# Write the maximum threshold value #
    def writeMaxFloat(self, max) :
        self.writeEncoderFloat(REG_CMAXB4, max)

# Write the minimum threshold value #
    def writeMin(self, min) :
        self.writeEncoder32(REG_CMINB4, min)

# Write the minimum threshold value #
    def writeMinFloat(self, min) :
        self.writeEncoderFloat(REG_CMINB4, min)

# Write the Step increment value #
    def writeStep(self, step):
        self.writeEncoder32(REG_ISTEPB4, step)

# Write the Step increment value #
    def writeStepFloat(self, step):
        self.writeEncoderFloat(REG_ISTEPB4, step)

# Write the PWM value of the RGB LED red #
    def writeLEDR(self, rled):
        self.writeEncoder8(REG_RLED, rled)

# Write the PWM value of the RGB LED green #
    def writeLEDG(self, gled):
        self.writeEncoder8(REG_GLED, gled)

# Write the PWM value of the RGB LED blue #
    def writeLEDB(self, bled):
        self.writeEncoder8(REG_BLED, bled)

# Write 24bit color code #
    def writeRGBCode(self, rgb):
        self.writeEncoder24(REG_RLED, rgb)

# Write GP1 register, used when GP1 is set to output or PWM #
    def writeGP1(self, gp1):
        self.writeEncoder8(REG_GP1REG, gp1)

# Write GP2 register, used when GP2 is set to output or PWM #
    def writeGP2(self, gp2):
        self.writeEncoder8(REG_GP2REG, gp2)

# Write GP3 register, used when GP3 is set to output or PWM #
    def writeGP3(self, gp3):
        self.writeEncoder8(REG_GP3REG, gp3)

# Write Anti-bouncing period register #
    def writeAntibouncingPeriod(self, bounce):
        self.writeEncoder8(REG_ANTBOUNC, bounce)

# Write Anti-bouncing period register #
    def writeDoublePushPeriod(self, dperiod):
        self.writeEncoder8(REG_DPPERIOD, dperiod)

# Write Fade timing in ms #
    def writeFadeRGB(self, fade):
        self.writeEncoder8(REG_FADERGB, fade)

# Write Fade timing in ms #
    def writeFadeGP(self, fade):
        self.writeEncoder8(REG_FADEGP, fade)

# Write the Gamma value on RLED #
    def writeGammaRLED(self, gamma):
        self.writeEncoder8(REG_GAMRLED, gamma)

# Write the Gamma value on GLED #
    def writeGammaGLED(self, gamma):
        self.writeEncoder8(REG_GAMGLED, gamma)

# Write the Gamma value on BLED #
    def writeGammaBLED(self, gamma):
        self.writeEncoder8(REG_GAMBLED, gamma)

# Write the Gamma value on GP1 #
    def writeGammaGP1(self, gamma):
        self.writeEncoder8(REG_GAMMAGP1, gamma)

# Write the Gamma value on GP2 #
    def writeGammaGP2(self, gamma):
        self.writeEncoder8(REG_GAMMAGP2, gamma)

# Write the Gamma value on GP3 #
    def writeGammaGP3(self, gamma):
        self.writeEncoder8(REG_GAMMAGP3, gamma)

# Write the EEPROM memory#
    def writeEEPROM(self, add, data):

        if add <= 0x7f:
            if (self.gconf & EEPROM_BANK1) != 0:
                self.gconf = self.gconf & 0xBF
                self.writeEncoder8(REG_GCONF, self.gconf)

            self.writeEncoder8((REG_EEPROMS + add), data)
        else:
            if (self.gconf & EEPROM_BANK1) == 0:
                self.gconf = self.gconf | 0x40
                self.writeEncoder8(REG_GCONF, self.gconf)

            self.writeEncoder8(add, data)

        sleep(0.001)

# Send to the encoder 1 byte #
    def writeEncoder8(self, add, value):
        self.i2cbus.write_byte_data(self.i2cadd, add, value)    
        return -1

# Send to the encoder 3 byte #
    def writeEncoder24(self, add, value):
        data = [0, 0, 0, 0]
        s = struct.pack('>i', value)
        data = struct.unpack('BBB', s[1:4])
        self.i2cbus.write_i2c_block_data(self.i2cadd, add, data)
        return -1
        
# Send to the encoder 4 byte #
    def writeEncoder32(self, add, value):
        data = [0, 0, 0, 0]
        s = struct.pack('>i', value)
        data = struct.unpack('BBBB', s)
        self.i2cbus.write_i2c_block_data(self.i2cadd, add, data)
        return -1
        
# Send to the encoder a float number #
    def writeEncoderFloat(self, add, value):
        data = [0, 0, 0, 0]
        s = struct.pack('>f', value)
        data = struct.unpack('BBBB', s)
        self.i2cbus.write_i2c_block_data(self.i2cadd, add, data)
        return -1
        
# read the encoder 1 byte #     
    def readEncoder8(self, add):
        data = [0]
        data[0] = self.i2cbus.read_byte_data(self.i2cadd, add)    
        value = struct.unpack(">b", bytearray(data))
        return value[0]
        
# read the encoder 2 byte #             
    def readEncoder16(self, add):
        data = [0, 0]
        data = self.i2cbus.read_i2c_block_data(self.i2cadd, add, 2)
        value = struct.unpack(">h", bytearray(data))
        return value[0] 
    
# read the encoder 4 byte #             
    def readEncoder32(self, add):
        data = [0, 0, 0, 0]
        data = self.i2cbus.read_i2c_block_data(self.i2cadd, add, 4)
        value = struct.unpack(">i", bytearray(data))
        return value[0]
        
# read the encoder float#             
    def readEncoderFloat(self, add):
        data = [0, 0, 0, 0]
        data = self.i2cbus.read_i2c_block_data(self.i2cadd, add, 4)
        value = struct.unpack(">f", bytearray(data)) 
        return value[0]
