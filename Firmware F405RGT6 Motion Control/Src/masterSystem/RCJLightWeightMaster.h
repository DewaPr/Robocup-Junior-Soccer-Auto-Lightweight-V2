#include "../u8g2/u8g2.h"

#ifndef _RCJ_LIGHT_WIUGHT_
#define _RCJ_LIGHT_WIUGHT_


// Defines // ---------------------------------------------------
#define true	1
#define false	0

#define BTN_PRESS_OK    0b10000000
#define BTN_PRESS_UP    0b01000000
#define BTN_PRESS_DOWN  0b00100000
#define BTN_PRESS_MINUS 0b00000100
#define BTN_PRESS_PLUS  0b00000010
#define BTN_PRESS_BACK  0b00000001

#define MENU_PAGE               0
#define RUNNING_MODE_PAGE       1
#define SETTINGS_PAGE           2
#define SENSOR_TEST_PAGE        3
#define CAMERA_TEST_PAGE        4
#define IMU_SENSOR_PAGE         5
#define BALL_SENSOR_PAGE        6
#define LINE_SENSOR_PAGE        7
#define BATTERY_CAL_PAGE        8
#define START_PAGE              200

#define ANIMATION_SLIDE_SPEED   0.4
#define ANIMATION_UPDATE_MILLIS 5

#define BALL_CATCH_MODE_AND    0
#define BALL_CATCH_MODE_OR     1

#define BALL_VALUE_UNSIGNED		0
#define BALL_VALUE_SIGNED		1

#define ROBOT_MODE_ATTACKER1 		1
#define ROBOT_MODE_ATTACKER2 		2
#define ROBOT_MODE_ATTACKER3 		3
#define ROBOT_MODE_DEFENDER  		10
#define ROBOT_MODE_PENALTY_SHOOT1 	20
#define ROBOT_MODE_PENALTY_SHOOT2  	21
#define ROBOT_MODE_PENALTY_KEEP   	30

#define PI 3.1415926535897932384626433832795

#define FEE_SECTOR_ADDRESS 0x08060000

#define CAM_X_TOTAL_PX 	650
#define CAM_Y_TOTAL_PX 	600
#define CAM_X_MIN_PX 	(CAM_Y_TOTAL_PX / -2)
#define CAM_X_MAX_PX 	(CAM_Y_TOTAL_PX / 2)

#define CAM_UART		1
#define IMU_UART		2

#define CAM_X_SUBSTRACT_VAL				250

#define SIDE_MOVEMENT_THETA				112
#define ZIGZAG_MOVEMENT_TIME			1000
#define SIDE_TIMEOUT_MOVEMENT_TIME		/*250*/ 0
#define HOLD_AFTER_SHOOT_MS				/*2000*/ 10000
#define BALL_CATCHED_TIMEOUT			/*4000*/ 10000

struct eepObject
{
  uint8_t robotSpeedVal;
  uint8_t kickerVal;
  uint8_t dribblerVal;
  uint16_t batteryCal;
  uint16_t lineSensorUp[24];
  uint16_t lineSensorDown[24];
};

typedef struct eepObject eepObject;

long map(long x, long in_min, long in_max, long out_min, long out_max);

#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)));

// Main System // ---------------------------------------------------
void systemBegin();
void systemLoop();
void displayUpdate();
void UART_DMA_Restart(uint16_t _uartX);

// Menu page // ---------------------------------------------------
void menuPage(int _gOff, int _lineOff);

// Battery Page // ---------------------------------------------------
void batteryCallPage(int _gOff);
void displayBattery(int _yOff);
void drawBatteryLayout(float _voltage, int _off);

// Actuator write // ---------------------------------------------------
void actuatorBegin();
void updateCCR(uint16_t _pin, uint8_t _val);
void writeMA(int _val);
void writeMB(int _val);
void writeMC(int _val);
void writeMD(int _val);
void setOmniValue(int _vd, int _thetaDeg, int _rotation);
void writeDR1(int _val);
void writeDR2(int _val);
void activateSolenoid();
void killAllMotor();
void actuatorLoop();

// Start page // ---------------------------------------------------
void robotStartBegin();
void setRobotMode(uint8_t _rm);
void startPage(int _gOff);

// Running page // ---------------------------------------------------
void runningModePage(int _gOff);

// Settings page // ---------------------------------------------------
void settingsPage(int _gOff);

// Sensor Test page // ---------------------------------------------------
void sensorTestPage(int _gOff);

// Camera test page // ---------------------------------------------------
void cameraTestPage();
void drawCameraLayout(int _gOff, int _px, uint8_t _det);

// IMU test page // ---------------------------------------------------
void IMUTestPage(int _gOff);
void drawImuYawLayout(int _gOff, float _deg);

// IR sensor test page // ---------------------------------------------------
void IRSensorTestPage(int _gOff);
void drawIRSensorLayout(int _gOff, uint16_t _valActive, uint8_t _valAuxActive);

// Line sensor test page // ---------------------------------------------------
void lineSensorTestPage(int _gOff);
void drawLineSensorLayout(int _gOff, uint8_t _frontActive, uint8_t _leftActive, uint8_t _backActive, uint8_t _rightActive);

// IMU Alg // ---------------------------------------------------
void imuBegin();
void imuLoop();
float getHeadingDegree();
void calibrateIMU();
uint8_t getImuDataValid();

// Cam Alg // ---------------------------------------------------
void camBegin();
void camLoop();
int getCamX();
int getCamWidth();
uint8_t getCamIdo();
uint8_t getCamDetect();
uint8_t getCamDataValid();

// Attacker Alg // ---------------------------------------------------
void attackerAlgorithm1();
void attackerAlgorithm2();
void attackerAlgorithm3();

// Defender Alg // ---------------------------------------------------
void defenderAlgorithm();

// Defender Alg // ---------------------------------------------------
void penaltyAlgorithmS1();
void penaltyAlgorithmS2();
void penaltyAlgorithmK();

// Function page // ---------------------------------------------------
void displayLoading();
void eepBegin();
void eepPutValue();
void eepGetValue();
float getBatteryVoltage();
uint16_t analogRead(uint16_t _pin);
void buttonBegin();
uint8_t getButtonState();
void drawArrowAnimation(int _x1GOff, int _x2GOff, int _off);
void drawBoxWithText(int _x, int _y, int _w, int _h, int _off, char *_str, uint8_t _selected);
void maskTittle();
uint8_t getCurrentPage();
void setCurrentPage(uint8_t _page);
uint8_t getPageUpdateFlag();
void resetPageUpdateFlag();

// Display Logo // ---------------------------------------------------
void displayLogo();
void drawLogo(int _off);

// Sensor read // ---------------------------------------------------
void sensorsBegin();
void sensorsLoop();
uint16_t getIRData();
uint8_t getIRAuxData();
float getBallPosition(int _bias, uint8_t _sig);
uint8_t getBallCatched(uint8_t _mode);
uint8_t *getUART1Data();
uint8_t *getUART2Data();
uint8_t getLineSensorBits(uint8_t _sel);


#endif
