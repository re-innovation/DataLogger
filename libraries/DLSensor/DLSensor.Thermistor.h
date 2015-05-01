#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

/*
 * Defines and typedefs
 */

class Thermistor
{

	public:
		Thermistor(float B, float R25, bool highSide);
		float TemperatureFromResistance(float R);
		float TemperatureFromADCReading(float otherResistor, float reading, uint16_t maxReading);
		float TemperatureFromADCReading(float otherResistor, uint16_t reading, uint16_t maxReading);
		float ResistanceFromTemperature(float t);
		float Rinf(void);

	private:
		float m_R25;
		float m_B;
		float m_Rinf;
		bool m_highSide;
};

Thermistor * getHighsideThermistor(float B, float R25);
Thermistor * getTLowsideThermistor(float B, float R25);

#endif
