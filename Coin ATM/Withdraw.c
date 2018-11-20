void withdraw(struct account)
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
			wait1Msec(500);
			displayBigTextLine(3, "Insufficient Funds");
		}
	} while (amount > account.accountBal);

	int num_100 = 0, num_25 = 0, num_10 = 0, num_5 = 0;
	outputCombo(amount, num_100, num_25, num_10, num_5)

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


