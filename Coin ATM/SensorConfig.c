const int BELT_SPEED = 0;
const tMotor BELT_MOTOR = motorA;
const int DISPENSE_SPEED = 0;
const tMotor DISPENSE_MOTOR = motorB;
const int BIN_SPEED = 0;
const tMotor BIN_MOTOR = motorC;

const tSensors CARD_COLOUR = S1;
const tSensors US_SENSOR = S2;
const tSensors COIN_COLOUR = S3;

void configureSensors()
{
	SensorType[CARD_COLOUR] = sensorEV3_Color;
	wait1Msec(50);
	SensorType[COIN_COLOUR] = sensorEV3_Color;
	wait1Msec(50);
	SensorType[US_SENSOR] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorMode[CARD_COLOUR] = modeEV3Color_Color;
	wait1Msec(50);
	SensorMode[COIN_COLOUR] = modeEV3Color_Color;
	wait1Msec(50);
}

task main()
{

}
