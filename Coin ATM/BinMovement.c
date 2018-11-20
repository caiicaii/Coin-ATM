const int ENC_LIMIT_1 = 30; //Bin 1 = $100
const int ENC_LIMIT_2 = 120; //Bin 2 = $25
const int ENC_LIMIT_3 = 210; //Bin 3 = $10
const int ENC_LIMIT_4 = 300; //Bin 4 = $5

void binMovement(int binNum)
{
	int encoderLimit = 0;
	switch (binNum)
	{
		case 1:
			encoderLimit = ENC_LIMIT_1;
			break;
		case 2:
			encoderLimit = ENC_LIMIT_2;
			break;
		case 3:
			encoderLimit = ENC_LIMIT_3;
			break;
		case 4:
			encoderLimit = ENC_LIMIT_4;
			break;
	}

	if (nMotorEncoder[BIN_MOTOR] < encoderLimit)
	{
		motor[BIN_MOTOR] = 20;
		while (nMotorEncoder[BIN_MOTOR] < encoderLimit)
		{}
	}
	else
	{
		motor[BIN_MOTOR] = -20;
		while (nMotorEncoder[BIN_MOTOR] > encoderLimit)
		{}
	}

	motor[BIN_MOTOR] = 0;

	wait1Msec(500);
}
