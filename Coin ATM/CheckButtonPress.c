tEV3Buttons get_button_press()
{
	while (!getButtonPress(buttonAny))
	{}

	tEV3Buttons buttonSave = buttonNone;

	for (TEV3Button button = buttonUp; button <= buttonLeft && buttonSave == buttonNone; button++)
		if (getButtonPress(button))
			buttonSave = button;

	while (getButtonPress(buttonAny))
	{}

	return buttonSave;
}

task main()
{

}
