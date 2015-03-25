#ifndef _GPS_UTILITY_H_
#define _GPS_UTILITY_H_

struct gps_data_struct
{
    float latitude;
    float longitude;
    float altitude;
    uint8_t dd;
    uint8_t mm;
    uint8_t yy;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t nsats;
};
typedef struct gps_data_struct GPS_DATA;

bool GPS_parseGPRMCSentence(const char * pSentence, GPS_DATA * pData);

#ifdef TEST
// Only expose these functions to the test harness
uint8_t parseHHMMSSTime(const char * pSentence, uint8_t * h, uint8_t * m, uint8_t * s);
uint8_t parseDDMMYYDate(const char * pSentence,  uint8_t * d, uint8_t * m, uint8_t * y);
uint8_t parseLatitude(const char * pSentence, float * pResult);
uint8_t parseLongitude(const char * pSentence, float * pResult);
#endif

#endif
