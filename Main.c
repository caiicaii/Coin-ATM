const int BELT_SPEED = 6;
const tMotor BELT_MOTOR = motorA;
const int DISPENSE_SPEED = 10;
const tMotor DISPENSE_MOTOR = motorB;
const int BIN_SPEED = 30;
const tMotor BIN_MOTOR = motorC;

const tSensors CARD_COLOUR = S1;
const tSensors US_SENSOR = S2;
const tSensors COIN_COLOUR = S3;

const int ENC_LIMIT_1 = 0; //Bin 1 = $100
const int ENC_LIMIT_2 = 165; //Bin 2 = $25
const int ENC_LIMIT_3 = 330; //Bin 3 = $10
const int ENC_LIMIT_4 = 495; //Bin 4 = $5

const int PATTERN_SIZE = 4;

const int AVG_REACH = 80;
const int COLOUR_TOL = 7;

bool cardInserted = false;
bool loginSuccess = false;
bool shutdown = false;
long COIN_RGB[3];

/*
Red = bin1
Blue = bin2
Green = bin3
Tape = bin4 */
//RGB Values need to be defined
//RGB value of coins, Red = index 0, blue = index 1, green = index 2, tape = index 3
const int RED_VALUES[4] = {42, 12, 11, 73};
const int GREEN_VALUES[4] = {15, 32, 21, 64};
const int BLUE_VALUES[4] = {9, 11, 16, 50};
const int WHITE_BACKGROUND[3] = {49, 54, 48}; //RGB values for the white card detection background

typedef struct
{
	string name;
	float accountBal;
	long red;
	long green;
	long blue;
	int pattern[PATTERN_SIZE];
} Account;

const int NUM_CARDS = 4;
const int redList[NUM_CARDS] = {70, 15, 12, 57};  //order of cards: red, green, blue, yellow
const int greenList[NUM_CARDS] = {18, 45, 27, 43};
const int blueList[NUM_CARDS] = {12, 14, 25, 7};
Account accountList[NUM_CARDS];
Account activeAccount;

void initializeAccounts()
{
	accountList[0].name = "Justin";
	accountList[0].accountBal = 0;
	accountList[0].red = 65;
	accountList[0].green = 11;
	accountList[0].blue = 17;
	accountList[0].pattern[0] = buttonDown;
	accountList[0].pattern[1] = buttonDown;
	accountList[0].pattern[2] = buttonUp;
	accountList[0].pattern[3] = buttonUp;

	accountList[1].name = "Josh";
	accountList[1].accountBal = 0;
	accountList[1].red = 15;
	accountList[1].green = 14;
	accountList[1].blue = 49;
	accountList[1].pattern[0] = buttonRight;
	accountList[1].pattern[1] = buttonDown;
	accountList[1].pattern[2] = buttonRight;
	accountList[1].pattern[3] = buttonUp;

	accountList[2].name = "Jun";
	accountList[2].accountBal = 0;
	accountList[2].red = 12;
	accountList[2].green = 27;
	accountList[2].blue = 25;
	accountList[2].pattern[0] = buttonUp;
	accountList[2].pattern[1] = buttonLeft;
	accountList[2].pattern[2] = buttonDown;
	accountList[2].pattern[3] = buttonRight;

	accountList[3].name = "Charlie";
	accountList[3].accountBal = 69;
	accountList[3].red = 57;
	accountList[3].green = 43;
	accountList[3].blue = 7;
	accountList[3].pattern[0] = buttonDown;
	accountList[3].pattern[1] = buttonDown;
	accountList[3].pattern[2] = buttonDown;
	accountList[3].pattern[3] = buttonDown;
}

void configureSensors()
{
	motor[BELT_MOTOR] = 0;
	wait1Msec(50);
	motor[BIN_MOTOR] = 0;
	wait1Msec(50);
	motor[DISPENSE_MOTOR] = 0;
	wait1Msec(50);
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
	eraseDisplay();
}

//figure out which bin the current colour corresponds to
int getColour(tSensors COIN_COLOUR)
{
	getColorRGB(COIN_COLOUR, COIN_RGB[0], COIN_RGB[1], COIN_RGB[2]);

	for(int val = 0; val < 4; val++)
	{
		if(fabs(COIN_RGB[0] - RED_VALUES[val]) < COLOUR_TOL && fabs(COIN_RGB[1] - GREEN_VALUES[val]) < COLOUR_TOL && fabs(COIN_RGB[2] - BLUE_VALUES[val]) < COLOUR_TOL)
		{
			return val + 1;
		}
	}
	return -1; //no bin found for colour
}

void standby()
{
	eraseDisplay();
	motor[BELT_MOTOR] = motor[DISPENSE_MOTOR] = motor[BIN_MOTOR] = 0;
	wait1Msec(50);
	while (SensorValue(US_SENSOR) > AVG_REACH)
	{}
	displayCenteredBigTextLine(2, "Insert card");
	wait1Msec(3000);

	long signinR = 0;
	long signinG = 0;
	long signinB = 0;
	getColorRGB(CARD_COLOUR, signinR, signinG, signinB);
	if ((fabs(signinR - WHITE_BACKGROUND[0]) > COLOUR_TOL) && (fabs(signinG - WHITE_BACKGROUND[1]) > COLOUR_TOL) && (fabs(signinB - WHITE_BACKGROUND[2]) > COLOUR_TOL))
	{
		cardInserted = true;
	}
}

TEV3Buttons buttonPress()
{
	while (!getButtonPress(buttonAny))
	{}

	TEV3Buttons buttonSave = buttonNone;

	for (TEV3Buttons button = buttonUp; button <= buttonAny && buttonSave == buttonNone; button++)
	{
		if (getButtonPress(button))
		{
			buttonSave = button;
		}
	}

	while (getButtonPress(buttonAny))
	{}

	return buttonSave;
}

void verifyID(long testR, long testG, long testB)
{
	bool cardVerified = false;

	for(int index = 0; index < NUM_CARDS && cardVerified == false; index++)
	{
		if((fabs(redList[index] - testR) < COLOUR_TOL) && (fabs(greenList[index] - testG) < COLOUR_TOL)
			&& (fabs(blueList[index] - testB) < COLOUR_TOL))
		{
			cardVerified = true;
			activeAccount = accountList[index];
			displayString(8, "%d", index);
			displayString(9, accountList[index].name);
			wait1Msec(1000);
		}
	}

	/*TEV3Buttons userInput[PATTERN_SIZE] = {buttonNone, buttonNone, buttonNone, buttonNone};
	eraseDisplay();     															//need to very whose account here
	wait1Msec(500);
	displayCenteredBigTextLine(2, "Please enter");
	displayCenteredBigTextLine(4, "your pattern");
	displayCenteredBigTextLine(6, "%d %d %d", testR, testG, testB);
	bool moreThanOnce = false;
	bool patternStatus = true;

	do
	{
		if (moreThanOnce == true)
		{
			eraseDisplay();
			wait1Msec(1000);
			displayCenteredBigTextLine(2, "Wrong Pattern");
			displayCenteredBigTextLine(6, "Please Try");
			displayCenteredBigTextLine(8, "Again");
		}
		for(int pressNum = 0; pressNum < PATTERN_SIZE; pressNum++)
		{
			userInput[pressNum] = buttonPress();
			displayString(pressNum+3, "%d", userInput[pressNum]);
		}

		for (int index = 0; index < PATTERN_SIZE && patternStatus == true; index++)
		{
			if (userInput[index] != activeAccount.pattern[index])
				patternStatus = false;
		}
		moreThanOnce = true;
	} while (patternStatus == false);

	if (activeAccount == 'B' && patternStatus == true)
	{
		shutdown = true;
	}*/

	loginSuccess = true;
}

void menuOptions()
{
	eraseDisplay();
	wait1Msec(200);
	displayCenteredBigTextLine(1, "Choose a button");
	displayString(3, "Left to Deposit");
	displayString(4, "Right to Withdraw");
	displayString(5, "Enter to View Balance");
	displayString(7, "Down to Log-off");
}

void outputCombo(int & withdrawAmt, int & numOf100, int & numOf25, int & numOf10, int & numOf5)
{
	numOf100 = withdrawAmt/100;
	withdrawAmt -= numOf100*100;

	numOf25 = withdrawAmt/25;
	withdrawAmt -= numOf25*25;

	numOf10 = withdrawAmt/10;
	withdrawAmt -= numOf10*10;

	numOf5 = withdrawAmt/5;
}

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
		motor[BIN_MOTOR] = BIN_SPEED;
		while (nMotorEncoder[BIN_MOTOR] < encoderLimit)
		{}
	}
	else
	{
		motor[BIN_MOTOR] = -BIN_SPEED;
		while (nMotorEncoder[BIN_MOTOR] > encoderLimit)
		{}
	}

	motor[BIN_MOTOR] = 0;
	wait1Msec(500);
}

void displayBal(Account account)
{
	eraseDisplay();
	wait1Msec(500);
	displayCenteredBigTextLine(4, ".  .  .");
	eraseDisplay();
	wait1Msec(500);
	displayCenteredTextLine(2, "Your Current");
	displayCenteredTextLine(3, "Balance Is:");
	wait1Msec(1000);
	displayCenteredBigTextLine(5, "%d", account.accountBal);
	wait1Msec(2000);
	displayString(7, "Press any button");
	displayString(8, "to continue...");
	while(!getButtonPress(buttonAny))
	{}
	while(getButtonPress(buttonAny))
	{}
}

void pushCoin(int numOfCoins, Account account, int coinValue)
{
	int numOutput = 0;
	nMotorEncoder[DISPENSE_MOTOR] = 0;
	while(numOutput < numOfCoins)
	{
		motor[DISPENSE_MOTOR] = DISPENSE_SPEED;
		while(nMotorEncoder[DISPENSE_MOTOR] < 90)
		{}
		motor[DISPENSE_MOTOR] = -DISPENSE_SPEED;
		while(nMotorEncoder[DISPENSE_MOTOR] > 0)
		{}
		motor[DISPENSE_MOTOR] = 0;
		account.accountBal -= coinValue;
		numOutput++;
	}
}

void withdraw(Account account)
{
	int amount = 0;

	do
	{
		eraseDisplay();
		displayString(1, "Enter Withdrawal Amount");
		displayString(2, "Press up/down for +/- 25");
		displayString(3, "Press right/left for +/- 5");
		displayString(4, "Press Enter To Withdraw.");

		while (!getButtonPress(buttonEnter))
		{
			if (getButtonPress(buttonUp))
			{
				while (getButtonPress(buttonUp))
				{}
				amount += 25;
			}

			else if (getButtonPress(buttonDown))
			{
				if (amount >= 25)
				{
					while (getButtonPress(buttonDown))
					{}
						amount -= 25;
				}
			}

			if (getButtonPress(buttonRight))
			{
				while (getButtonPress(buttonRight))
				{}
				amount += 5;
			}

			else if (getButtonPress(buttonLeft))
			{
				if (amount >= 5)
				{
					while (getButtonPress(buttonLeft))
					{}
					amount -=5;
				}
			}
			displayCenteredBigTextLine(6, "            ");
			displayCenteredBigTextLine(6, "$%d", amount);
		}
		if (amount > account.accountBal)
		{
			eraseDisplay();
			wait1Msec(1000);
			displayBigTextLine(3, "Insufficient Funds");
		}
	} while (amount > account.accountBal);

	int num_100 = 0, num_25 = 0, num_10 = 0, num_5 = 0;
	outputCombo(amount, num_100, num_25, num_10, num_5);

	for (int bin = 1; bin <= 4; bin++)
	{
		int pushes = 0;
		int coinValue = 0;
		switch(bin)
		{
		case 1: binMovement(1);
			pushes = num_100;
			coinValue = 100;
			break;
		case 2: binMovement(2);
			pushes = num_25;
			coinValue = 25;
			break;
		case 3: binMovement(3);
			pushes = num_10;
			coinValue = 10;
			break;
		case 4: binMovement(4);
			pushes = num_5;
			coinValue = 5;
			break;
		}
		pushCoin(pushes, account, coinValue);
	}
	motor[BIN_MOTOR] = -BIN_SPEED;
	while (nMotorEncoder[BIN_MOTOR] > 0)
	{}
	motor[BIN_MOTOR] = 0;

	eraseDisplay();
	displayCenteredTextLine(1, "Remember to Collect");
	displayCenteredTextLine(2, "Your Coins!");
	wait1Msec(500);
	displayCenteredTextLine(4, "Your New Balance Is:");
	wait1Msec(1000);
	displayCenteredBigTextLine(6, "$%d", account.accountBal);
	wait1Msec(1000);
	displayCenteredTextLine(8, "Have a Good Day!");
	wait1Msec(2000);
}

void deposit(Account account)
{
	int match = -1;
	int sumDeposited = 0;
	bool pressedEnter = false;
	motor[BELT_MOTOR] = BELT_SPEED;
	do
	{
		time1[T1] = 0;
		while (time1[T1] < 5000)
		{
			if (getColorName(COIN_COLOUR) != colorBlack)
			{
				time1[T1] = 0;
				match = getColour(COIN_COLOUR);

				switch(match)
				{
				case 1: binMovement(1);
					account.accountBal += 100;
					displayCenteredBigTextLine(2, "                    ");
					sumDeposited += 100;
					displayCenteredBigTextLine(2, "%d", sumDeposited);
					break;
				case 2: binMovement(2);
					account.accountBal += 25;
					displayCenteredBigTextLine(2, "                    ");
					sumDeposited += 25;
					displayCenteredBigTextLine(2, "%d", sumDeposited);
					break;
				case 3: binMovement(3);
					account.accountBal += 10;
					displayCenteredBigTextLine(2, "                    ");
					sumDeposited += 10;
					displayCenteredBigTextLine(2, "%d", sumDeposited);
					break;
				case 4: binMovement(4);
					account.accountBal += 5;
					displayCenteredBigTextLine(2, "                    ");
					sumDeposited += 5;
					displayCenteredBigTextLine(2, "%d", sumDeposited);
					break;
				}
				while (getColorName(COIN_COLOUR) != colorBlack)
				{}
			}
		}
		motor[BELT_MOTOR] = 0;
		eraseDisplay();
		wait1Msec(500);
		displayString(2, "Are you done depositing coins?");
		displayString(3, "Press enter to continue.");
		displayString(4, "Press any other to end.");
		while(!getButtonPress(buttonAny))
		{}
		pressedEnter = getButtonPress(buttonEnter);
		while(getButtonPress(buttonAny))
		{}
	} while(pressedEnter == true);

	motor[BIN_MOTOR] = -BIN_SPEED;
	while (nMotorEncoder[BIN_MOTOR] > 0)
	{}
	motor[BIN_MOTOR] = 0;

	eraseDisplay();
	displayCenteredTextLine(2, "Total Amount Deposited:");
	displayCenteredBigTextLine(4, "%d", sumDeposited);
}

task main()
{
	initializeAccounts();
	configureSensors();
	const TEV3Buttons blackPattern[PATTERN_SIZE] = {buttonEnter, buttonEnter, buttonEnter, buttonEnter};
	long testR = 0;
	long testG = 0;
	long testB = 0;
	long discardR = 0;
	long discardG = 0;
	long discardB = 0;
	wait1Msec(100);
	getColorRGB(CARD_COLOUR, discardR, discardG, discardB);
	wait1Msec(100);

	do
	{
		while (loginSuccess == false)
		{
			while (cardInserted == false)
			{
				standby();
			}
			getColorRGB(CARD_COLOUR, testR, testG, testB);

			verifyID(testR, testG, testB);
			displayString(7, "%d %d %d", testR, testG, testB);
			wait1Msec(2000);
		}

		bool down = false;
		do
		{
			wait1Msec(500);
			if (fabs(testR - 5) < COLOUR_TOL && fabs(testG - 5) < COLOUR_TOL && fabs(testR-5) < COLOUR_TOL)
			{
					shutdown = true;
			}
			menuOptions();

			while(!getButtonPress(buttonAny))
			{}
			bool left = getButtonPress(buttonLeft);
			bool right = getButtonPress(buttonRight);
			down = getButtonPress(buttonDown);
			bool enter = getButtonPress(buttonEnter);
			if (left)
			{
				eraseDisplay();
				wait1Msec(1000);
				displayCenteredBigTextLine(2, "Please Deposit");
				displayCenteredBigTextLine(4, "Your Coins");
				deposit(activeAccount);
			}
			else if (right)
			{
				wait1Msec(1000);
				withdraw(activeAccount);
			}
			else if (enter)
			{
				displayBal(activeAccount);
			}
		} while (down == false);

		eraseDisplay();
		displayString(2, "You have successfully");
		displayString(3, "logged out.");
		wait1Msec(500);
		displayString(4, "Thank you for using");
		displayCenteredBigTextLine(6, "420 ATM");
		wait1Msec(2000);
		eraseDisplay();
		loginSuccess = false;
		cardInserted = false;
	} while (shutdown == false);

	displayString(2, "ATM has been shut-down");
	displayString(3, "Thank you for using");
	wait1Msec(1000);
	displayCenteredBigTextLine(6, "420 ATM");
	wait1Msec(5000);
}
