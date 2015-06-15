/*
 * DLUtility.PD.cpp
 *
 * Potential divider routines
 *
 * Author: James Fowkes
 *
 * www.re-innovation.co.uk
 */

/*
 * PD_GetInputVoltage
 *
 * Assuming that the voltage in is from a potential divider
 * of resistors r1 and r2, calculate the divider input voltage
 *
 * in: The input voltage
 * r1: The "top" resistor in the divider
 * r2: The "bottom" resistor in the divider
 */

float PD_GetInputVoltage(float in, float r1, float r2)
{
	return (in * (r1 + r2)) / r2;
}

/*
 * PD_GetLowerResistance
 *
 * Assuming that the voltage in is from a potential divider
 * of resistors r1 and r2, calculate r2
 *
 * upperR: The "top" resistor in the divider
 * Vin: The input voltage to the divider
 * Vmid: The voltage read in the middle of the divider
 */
float PD_GetLowerResistance(float upperR, float Vin, float Vmid)
{
	return upperR * (1.0 / ((Vin / Vmid) - 1));
}
