#ifndef _UTILITY_POTENTIALDIVIDER_H_
#define _UTILITY_POTENTIALDIVIDER_H_

float PD_GetInputVoltage(float in, float r1, float r2);
float PD_GetLowerResistance(float upperR, float Vin, float Vmid);

#endif
