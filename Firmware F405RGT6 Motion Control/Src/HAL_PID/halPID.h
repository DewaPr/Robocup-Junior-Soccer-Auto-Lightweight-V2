#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "math.h"

#define P_ON_E 	0
#define P_ON_M 	1

#define OFF		0
#define ON		1

#define FORWARD		0
#define BACKWARD	1


void halPIDbegin(  const uint8_t _pOn,
				   const uint8_t _direction,
				   const unsigned int _minSamplePeriodMs,
				   const double _bias);
void halPIDstart();
void halPIDreset();
void halPIDstop();
double halPIDcompute(double _p, double _i, double _d, double _input, double _setpoint);
void halPIDdoCompute(uint32_t timeDiff);
void halPIDsetOutputLimits(const double min, const double max);
void halPIDsetWindUpLimits(const double min, const double max);
void halPIDsetDeadBand(const double min, const double max);
void halPIDsetPOn(const uint8_t _pOn);
void halPIDsetBias(const double _bias);
void halPIDsetCoefficients(const double _p, const double _i, const double _d);
void halPIDsetDirection(const uint8_t _direction);
void halPIDreverse();
void halPIDsetSampleTime(const unsigned int _minSamplePeriodMs);

double B();
double P();
double I();
double D();
