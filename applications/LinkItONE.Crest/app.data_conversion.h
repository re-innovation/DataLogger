#ifndef _APP_DATA_CONVERSION_H_
#define _APP_DATA_CONVERSION_H_

typedef float (*CONVERSION_FN)(uint16_t in);

float channel01Conversion(uint16_t in);
float channel02Conversion(uint16_t in);
float channel03Conversion(uint16_t in);
float channel04Conversion(uint16_t in);
float channel05Conversion(uint16_t in);
float channel06Conversion(uint16_t in);
float channel07Conversion(uint16_t in);
float channel08Conversion(uint16_t in);
float channel09Conversion(uint16_t in);
float channel10Conversion(uint16_t in);
float channel11Conversion(uint16_t in);
float channel12Conversion(uint16_t in);

#endif
