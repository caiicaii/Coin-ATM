void pushCoin(int numOfCoins, struct account, int coinValue)
{
	int numOutput = 0;
	nMotorEncoder[DISPENSE_MOTOR] = 0;
	while(numOutput < numOfCoins)
	{
		motor[DISPENSE_MOTOR] = 25;
		while(nMotorEncoder[DISPENSE_MOTOR] < 360)
		{}
		motor[DISPENSE_MOTOR] = 0;
		account.accountBal -= coinValue;
		nMotorEncoder[DISPENSE_MOTOR] = 0;
		numOutput++;
	}
}
