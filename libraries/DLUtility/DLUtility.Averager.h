#ifndef _DL_AVERAGER_H_
#define _DL_AVERAGER_H_

/*
 * Defines and Typedefs
 */

enum integertype
{
	S8,
	U8,
	S16,
	U16,
	S32,
	U32
};
typedef enum integertype INTEGERTYPE;

typedef struct averager AVERAGER;

AVERAGER * AVERAGER_GetAverager(INTEGERTYPE eType, uint8_t size);

void AVERAGER_NewData(AVERAGER * pAverager, void * pNewData);

void AVERAGER_GetAverage(AVERAGER * pAverager, void * pResult);

void AVERAGER_Reset(AVERAGER * pAverager, void * pValue);

#endif
