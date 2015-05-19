#ifndef _DATAFIELD_CONVERSION_H_
#define _DATAFIELD_CONVERSION_H_

float CONV_VoltsFromRaw(float raw, VOLTAGECHANNEL * conversionData);
float CONV_AmpsFromRaw(float raw, CURRENTCHANNEL * conversionData);
float CONV_CelsiusFromRawThermistor(float raw, THERMISTORCHANNEL * conversionData);

#endif
