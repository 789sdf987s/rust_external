/**
 * <3 skit#2567
 * 
 * don't make this into a shitty p2c
 */

#pragma once

class Menu {

public:
	void Watermark();
	void Render();
	void Shutdown();
	void ColorPicker(const char* name, float* color, bool alpha);

	void Aimbot();
	void Visuals();
	void Misc();
	void Players();

	bool isOpen = false;
};

extern Menu* menu;