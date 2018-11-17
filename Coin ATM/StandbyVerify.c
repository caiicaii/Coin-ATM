const int AVG_REACH = 85;
const int COLOUR_TOL = 25;

bool loginSuccess = false; // global variable
char activeAccount = ''; // global variable

void standby()
{
	eraseDisplay();
	motor[BELT_MOTOR] = motor[DISPENSE_MOTOR] = motor[BIN_MOTOR] = 0;
	while (SensorValue(US_SENSOR) > AVG_REACH)
	{}
}

void verifyID()
{
	long testR = 0;
	long testG = 0;
	long testB = 0;

	displayString(1, "Welcome");
	wait1Msec(3000);
	displayString(2, "Please insert your user card");

	bool cardVerified == false;

	while(getColorName(CARD_COLOUR) == colorWhite)
	{}
	getColorRGB(CARD_COLOUR, testR, testG, testB);
	for(int index = 0; index < sizeof(accountList) && cardVerified == false; index++)
	{
		if((fabs(accountList.red - testR) < COLOUR_TOL) && (fabs(accountList.green - testG) < COLOUR_TOL)
			&& (fabs(accountList.blue - testB) < COLOUR_TOL))
		{
			cardVerified = true;
			activeAccount = accountList[index];
		}
	}

	tEV3Buttons userInput[PATTERN_SIZE] = {0};
	eraseDisplay();     															//need to very whose account here
	displayString(2, "Please enter your pattern");
	bool moreThanOnce = false;

	do
	{
		if (moreThanOnce == true)
		{
			eraseDisplay();
			wait1Msec(500);
			displayString(2, "Pattern Incorrect");
			displayString(3, "Please Try Again");
		}
		for(int pressNum = 0; pressNum < PATTERN_SIZE; pressNum++)
		{
			userInput[pressNum] = get_button_press();
		}

		bool patternStatus = true;
		for (int index = 0; index < PATTERN_SIZE && patternStatus == true; index++)
		{
			if (userInput[index] != pattern[index])
				patternStatus = false;
		}
		moreThanOnce = true;
	} while (patternStatus == false);

	loginSuccess = true;
}

task main()
{

}
