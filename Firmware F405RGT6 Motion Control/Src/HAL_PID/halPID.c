#include "main.h"
#include "halPID.h"
#include "../masterSystem/RCJLightWeightMaster.h"
double bias;

double outputMax = 255;
double outputMin = 0;

double windupMax = 1000;
double windupMin = -1000;

double deadBandMax = 0;
double deadBandMin = 0;

double curError;
double curSetpoint;
double curInput;

double lastError;
double lastSetpoint;
double lastInput;

double pIn;
double iIn;
double dIn;

double kp;
double ki;
double kd;

double pOut;
double iOut;
double dOut;

unsigned long timerIntervalMillis;
unsigned long sampleTime;

double input;
double output;
double setpoint;

uint8_t pOnType;
uint8_t modeType;
uint8_t direction;

void halPIDbegin(  const uint8_t _pOn,
				   const uint8_t _direction,
				   const unsigned int _minSamplePeriodMs,
				   const double _bias)
{
	halPIDsetPOn(_pOn);
	halPIDsetBias(_bias);
	halPIDsetDirection(_direction);
	halPIDsetSampleTime(_minSamplePeriodMs);

	halPIDreset();
	halPIDstart();
}





void halPIDstart()
{
	if (modeType != ON)
	{
		modeType = ON;
		halPIDreset();
	}
}




void halPIDreset()
{
	curError    = 0;
	curSetpoint = 0;
	curInput    = 0;

	lastError    = 0;
	lastSetpoint = 0;
	lastInput    = 0;

	pOut = 0;
	iOut = 0;
	dOut = 0;

	timerIntervalMillis = HAL_GetTick();
}





void halPIDstop()
{
	if (modeType != OFF)
		modeType = OFF;
}




void halPIDdoCompute(uint32_t timeDiff) {
    kp = pIn;
    if (timeDiff > 0) {
        ki = iIn * (timeDiff / 1000.0);
        kd = dIn / (timeDiff / 1000.0); // go to inf if timeDiff == 0
    } else {
        ki = 0.0;
        kd = 0.0;
    }

    if (direction == BACKWARD)
    {
        kp *= -1;
        ki *= -1;
        kd *= -1;
    }

    lastInput    = curInput;
    lastSetpoint = curSetpoint;
    lastError    = curError;

    curInput    = input;
    curSetpoint = setpoint;
    curError    = curSetpoint - curInput;

    double dInput = input - lastInput;

    if (pOnType == P_ON_E)
        pOut = kp * curError;
    else if (pOnType == P_ON_M)
        pOut = -kp * dInput;

    dOut = -kd * dInput; // Derrivative on measurement

    double iTemp = (iIn == 0.0) ? 0.0 : iOut + (ki * ((curError + lastError) / 2.0)); // Trapezoidal integration
    iTemp        = constrain(iTemp, windupMin, windupMax);       // Prevent integral windup

    double outTemp = bias + pOut + dOut;                           // Output without integral
    double iMax    = constrain(outputMax - outTemp, 0, outputMax); // Maximum allowed integral term before saturating output
    double iMin    = constrain(outputMin - outTemp, outputMin, 0); // Minimum allowed integral term before saturating output

    iOut = constrain(iTemp, iMin, iMax);

    outTemp += iOut;
    outTemp = constrain(outTemp, outputMin, outputMax);
    output  = outTemp;

}




double halPIDcompute(double _p, double _i, double _d, double _input, double _setpoint)
{
	halPIDsetCoefficients(_p, _i, _d);
	input    = _input;
	setpoint = _setpoint;
	unsigned long timeDiff = HAL_GetTick() - timerIntervalMillis;
	if (timeDiff > sampleTime && modeType == ON)
	{
		halPIDdoCompute(timeDiff);
	}
	return output;
}




void halPIDsetOutputLimits(const double min, const double max)
{
	if (max > min)
	{
		outputMax = max;
		outputMin = min;

		if (modeType == ON)
			output = constrain(output, outputMin, outputMax);
	}
}




void halPIDsetWindUpLimits(const double min, const double max)
{
	if (max > min)
	{
		windupMax = max;
		windupMin = min;
	}
}




void halPIDsetDeadBand(const double min, const double max)
{
	if (max >= min)
	{
		deadBandMax = max;
		deadBandMin = min;
	}
}




void halPIDsetPOn(const uint8_t _pOn)
{
	pOnType = _pOn;
}




void halPIDsetBias(const double _bias)
{
	bias = _bias;
}




void halPIDsetCoefficients(const double _p, const double _i, const double _d)
{
	if (_p >= 0 && _i >= 0 && _d >= 0)
	{
		pIn = _p;
		iIn = _i;
		dIn = _d;
	}
}




void halPIDsetDirection(const uint8_t _direction)
{
	direction = _direction;

	if (modeType == ON)
		halPIDreset();
}




void halPIDreverse()
{
	if (direction == FORWARD)
		direction = BACKWARD;
	else if (direction == BACKWARD)
		direction = FORWARD;

	if (modeType == ON)
		halPIDreset();
}




void halPIDsetSampleTime(const unsigned int _minSamplePeriodMs)
{
	sampleTime = _minSamplePeriodMs;
}




double halPIDB()
{
	return bias;
}




double halPIDP()
{
	return pOut;
}




double halPIDI()
{
	return iOut;
}




double halPIDD()
{
	return dOut;
}
