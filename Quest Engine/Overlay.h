#pragma once

/* A 2D interface displayed on top of the viewport. */
class Overlay
{
public:
	virtual void onKeyboard(unsigned char, int, int);
	virtual void onSpecialInput(int, int, int);

private:
	Overlay();
};
