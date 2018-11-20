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
