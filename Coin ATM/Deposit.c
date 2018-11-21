/*
 Red = bin1
 Blue = bin2
 Green = bin3
 Tape = bin4 */
// Needs to be defined
const int RED_VALUES[4];
const int GREEN_VLAUES[4];
const int BLUE_VALUES[4];
const int COLOUR_TOL;

void deposit(struct account)
{
	motor[COIN_COLOUR] = 10;
	int match = -1;
	long RGB[3];
	int sumDeposited = 0;
	do
	{
		timer[T1] = 0;
		while (timer[T1] < 5000)
		{
			if (getColourName(COIN_COLOUR) != colorBlack || getColourName(COIN_COLOUR) != colorWhite)
			{
				timer[T1] = 0;
				getColorRGB(COIN_COLOUR, RGB[0], RGB[1], RGB[2]);

				for(int val = 0; val < 4; val++)
				{
					if(fabs(RGB[0] - RED_VALUES[val) < COLOUR_TOL && fabs(RGB[1] - GREEN_VALUES[val]) < COLOUR_TOL && fabs(RGB[2] - BLUE_VALUES[val]) < COLOUR_TOL)
					{
						match = val + 1;
					}
				}

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
			}
		}

		eraseDisplay();
		displayString(2, "Are you done depositing coins?");
		displayString(3, "Press enter to continue.");
		displayString(4, "Press any other to end.");
		while(!getButtonPress(buttonEnter) || !getButtonPress(buttonDown))
		{}
		bool pressedEnter = getButtonPress(buttonEnter);
		while(getButtonPress(buttonEnter) || !getButtonPress(buttonDown))
		{}
	} while(pressedEnter == true);

	eraseDisplay();
	displayCenteredTextLine(2, "Total Amount Deposited:");
	displayCenteredBigTextLine(4, "%d", sumDeposited);
}

