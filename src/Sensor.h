/* ===================================================
 * Sensor.h
 * ---------------------------------------------------
 * Sensor decoding from 433 Message
 *  Modified on Jan 15 2015
 *  Added OSV3 support for OWL CMR180 Energy sensor -- Onlinux (onlinux.fr)
 * 
 *  Created on: 17 sept. 2013
 *  Author: disk91 / (c) myteepi.com
 * ===================================================
 */

#ifndef SENSOR_H_
#define SENSOR_H_

#define SENS_CLASS_NONE	-1			// Not initialized
#define SENS_CLASS_MTP	 0			// MyTeePi virtual or physical sensor
#define SENS_CLASS_OS	 1			// Oregon Scientific

#define SENS_TYP_MTP_CPU	0		// cpu temperature
#define SENS_TYP_MTP_INT	1		// internal temperature

#define SENS_TYP_OS_1D20	0x1D20	// THGR122NX
#define SENS_TYP_OS_EC40	0xEC40	// THN132N
#define SENS_TYP_OS_1D30 	0x1D30  // THGRN228NX
#define SENS_TYP_OS_3D00 	0x3D00  // WGR9180
#define SENS_TYP_OS_2D10 	0x2D10  // STR928N
#define SENS_TYP_OS_5D60 	0x5D60  // BTHG968

class Sensor {

protected:
    int     power;
    double  powerTotal;
    double  rawpower;
	double	temperature;
	double	humidity;
	double  speed;
	double  direction;
	double  rain;
//	double  train;
	double	pressure;

	int 	channel;
	bool	battery;			// true if flag set (battery low)
	
	bool    havePower;          // true when power capacity decoded
	bool    haveRawPower;       // true when raw power capacity decoded
    bool    havePowerTotal;     // true when powerTotal capacity decoded
    
	bool	haveTemperature;	// true when temp capaciy decoded
	bool	haveHumidity;		// true when hum capacity decoded
	bool	haveSpeed;			// true when hum capacity decoded
	bool 	haveDirection;		// true when hum capacity decoded
	bool	haveRain;			// true when hum capacity decoded
//	bool	haveTrain;			// true when hum capacity decoded
	bool	havePressure;		// true when hum capacity decoded
	bool	haveBattery;		// true when battery flag decoded
	bool 	haveChannel;		// true when channel is present
	bool	isValid;			// true when chaecksum is valid and other value valid

	int		sensorClass;		// marque du sensor cf #define
	int		sensorType;			// model of sensor
	char    sensorName[128];	// name of the sensor

	time_t  creationTime;		// objectCreation time

	static char	_hexDecod[];
	virtual bool decode ( char * _str) = 0 ;		// decode the string and set the variable

protected:
	int getIntFromChar(char c);		// transform a Hex value in char into a number
	int getIntFromString(char *);	// transform a Hex value in String into a number
	double getDoubleFromString(char *);	// transform a BCD string into a double



public:

	Sensor( char * _strval );	// construct and decode value
    
    bool availablePower();      // return true if valid && have Power
    bool availableRawPower();   // return true if valid && have rawPower
    bool availablePowerTotal(); // return true if valid && have PowerTotal
    
	bool availableTemp();		// return true if valid && have Temp
	bool availableHumidity();	// return true if valid && have Humidity
	bool availablePressure();	// return true if valid && have Pressure
	bool availableSpeed();		// return true if valid && have Wind Speed
	bool availableDirection();	// return true if valid && have Wind Direction
	bool availableRain();		// return true if valid && have Rain
	bool isBatteryLow();		// return true if valid && haveBattery && flag set.
	bool hasChannel();			// return true if valid && haveChannel
	bool isDecoded();			// return true if valide

    double getPower();          // return power in kW
    double getRawPower();       // return power in kW
    double getPowerTotal();     // return power in kWh
    
	double getTemperature();	// return temperature in C°
	double getHumidity();		// return humidity in % (base 100)
	double getPressure();		// return pressure
	double getSpeed();			// return wind speed
	double getDirection();		// return wind direction
	double getRain();			// return rain
	int getChannel();			// return channel value

	int getSensClass();			// return sensor class
	int getSensType();			// return sensor type
	char * getSensName();		// return sensor name string

	time_t getCreationTime();	// return object creation time

	static Sensor * getRightSensor(char * s);	// wrapper for child class

};


class OregonSensorV2 : public Sensor {
	public :
		OregonSensorV2( char * _strval );

	private:
		bool decode( char * _str );			// wrapper to right decode method
		bool decode_THGR122NX(char * pt); 	// decode sensor informations
		bool decode_THN132N(char * pt);		// decode sensor informations
		bool decode_THGRN228NX(char * pt);
		bool decode_WGR918(char * pt);
		bool decode_BTHG968(char * pt);
		bool decode_RGR918(char * pt);

		bool validate(char * _str, int _len, int _CRC, int _SUM);	// Verify CRC & CKSUM

};

class OregonSensorV3 : public Sensor {
    public :
        OregonSensorV3( char * _strval );

    private:
        bool decode( char * _str );         // wrapper to right decode method
        bool decode_OWLCM180(char * pt);   // decode sensor informations

        bool validate(char * _str, int _len, int _CRC, int _SUM);   // Verify CRC & CKSUM

};
#endif /* SENSOR_H_ */
