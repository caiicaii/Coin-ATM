void displayBal(struct account)
{
	eraseDisplay();
	wait1Msec(500);
	displayCenteredBigTextLine(4, ".  .  .");
	eraseDisplay();
	wait1Msec(500);
	displayCenteredTextLine(2, "Your Current");
	displayCenteredTextLine(3, "Balance Is:");
	wait1Msec(1000);
	displayCenteredBigTextLine(5, "%d" account.accountBal);
	wait1Msec(2000);
	displayString(7, "Press any button");
	displayString(8, "to continue...");
	while(!getButtonPress(buttonAny)
	{}
	while(getButtonPress(buttonAny)
	{}
}


