#ifndef _PLATFORM_IF_H_
#define _PLATFORM_IF_H_

void PLATFORM_specialFieldSetup(NumericDataField * field);

#ifdef __LINKIT_ONE__
void PLATFORM_LINKITONE_specialFieldSetup(NumericDataField * field);
#endif

#endif