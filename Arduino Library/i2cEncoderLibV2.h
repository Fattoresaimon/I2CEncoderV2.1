//
//    FILE: i2cEncoderLibV2.h
// VERSION: 0.1..
// PURPOSE: Libreary for the i2c encoder board with arduinp
// LICENSE: GPL v3 (http://www.gnu.org/licenses/gpl.html)
//
// DATASHEET:
//
//     URL:
//
// AUTHOR:
// Simone Caron
//

#ifndef i2cEncoderLibV2_H
#define i2cEncoderLibV2_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

/*Encoder register definition*/
#define REG_GCONF  0x00
#define REG_GP1CONF  0x01
#define REG_GP2CONF  0x02
#define REG_GP3CONF  0x03
#define REG_INTCONF  0x04
#define REG_ESTATUS  0x05
#define REG_GPSTATUS  0x06
#define REG_CVALB4  0x07
#define REG_CVALB3  0x08
#define REG_CVALB2  0x09
#define REG_CVALB1  0x0A
#define REG_CMAXB4  0x0B
#define REG_CMAXB3  0x0C
#define REG_CMAXB2  0x0D
#define REG_CMAXB1  0x0E
#define REG_CMINB4  0x0F
#define REG_CMINB3  0x10
#define REG_CMINB2  0x11
#define REG_CMINB1  0x12
#define REG_ISTEPB4  0x13
#define REG_ISTEPB3  0x14
#define REG_ISTEPB2  0x15
#define REG_ISTEPB1  0x16
#define REG_RLED  0x17
#define REG_GLED  0x18
#define REG_BLED  0x19
#define REG_GP1REG  0x1A
#define REG_GP2REG  0x1B
#define REG_GP3REG  0x1C
#define REG_TEMP  0x1D
#define REG_EEPROMS  0x80

/* Encoder configuration bit */
#define FLOAT_DATA		0x01
#define INT_DATA    	0x00

#define WRAP_ENABLE		0x02
#define WRAP_DISABLE	0x00

#define DIRE_LEFT		0x04
#define DIRE_RIGHT		0x00

#define IPUP_DISABLE  0x08
#define IPUP_ENABLE 	0x00

#define RMOD_X2			0x10
#define RMOD_X1			0x00

#define RGB_ENCODER          0x20
#define STANDARD_ENCODER     0x00

#define EEPROM_BANK1     0x40
#define EEPROM_BANK2     0x00

#define RESET			0x80

/* Encoder status bit    */
#define E_PUSH 0x01
#define E_INCREMENT 0x02
#define E_DECREMENT 0x04
#define E_MAXVALUE 0x08
#define E_MINVALUE 0x10

#define S_PUSHR  0x01
#define S_PUSHP 0x02
#define S_RINC  0x04
#define S_RDEC  0x08
#define S_RMAX  0x10
#define S_RMIN  0x20
#define S_IGP  0x80

#define GP_PWM 0x00
#define GP_OUT 0x01
#define GP_AN 0x02
#define GP_IN 0x03
#define GP_PUP_EN 0x04
#define GP_PUP_DI 0x00
#define GP_INT_DI 0x00
#define GP_PUP_PE 0x08
#define GP_PUP_NE 0x10
#define GP_PUP_BE 0x18

union Data_v {
	float fval;
	int32_t val;
	uint8_t bval[4];
};

class i2cEncoderLibV2
{
public:
	i2cEncoderLibV2(uint8_t add);
	void begin( uint8_t conf);

	uint8_t readGP1conf(void);
	uint8_t readGP2conf(void);
	uint8_t readGP3conf(void);
	uint8_t readInterruptConfig(void);

	bool updateStatus(void);
	bool readStatus(uint8_t s);
	uint8_t readStatus(void);

	float readCounterFloat(void);
	int32_t readCounterLong(void);
	int16_t readCounterInt(void);
	int8_t readCounterByte(void);
	int32_t readMax(void);
	int32_t readMin(void);
	float readMaxFloat(void);
	float readMinFloat(void);
	int32_t readStep(void);
	float readStepFloat(void);

	uint8_t readEEPROM(uint8_t add);

	uint8_t readLEDR(void);
	uint8_t readLEDG(void);
	uint8_t readLEDB(void);

	uint8_t readGP1(void);
	uint8_t readGP2(void);
	uint8_t readGP3(void);

	/**    Write functions   **/
	void writeGP1conf(uint8_t gp1);
	void writeGP2conf(uint8_t gp2);
	void writeGP3conf(uint8_t gp3);
	void writeInterruptConfig(uint8_t interrupt);

	void writeCounter(int32_t counter);
	void writeMax(int32_t max);
	void writeMin(int32_t min);
	void writeStep(int32_t step);

	void writeCounter(float counter);
	void writeMax(float max);
	void writeMin(float min);
	void writeStep(float step);

	void writeLEDR(uint8_t rled);
	void writeLEDG(uint8_t gled);
	void writeLEDB(uint8_t bled);

	void writeGP1(uint8_t gp1);
	void writeGP2(uint8_t gp2);
	void writeGP3(uint8_t gp3);

	void writeEEPROM(uint8_t add, uint8_t data);

private:
	uint8_t _add = 0x00;
	uint8_t _stat = 0x00;
	uint8_t _gconf = 0x00;
	union Data_v _tem_data;
	uint8_t readEncoderByte(uint8_t reg);
	int16_t readEncoderInt(uint8_t reg);
	int32_t readEncoderLong(uint8_t reg);
	float readEncoderFloat(uint8_t reg);
	void writeEncoder(uint8_t reg, uint8_t data);
	void writeEncoder(uint8_t reg, int32_t data);
	void writeEncoder(uint8_t reg, float data);

};

#endif
