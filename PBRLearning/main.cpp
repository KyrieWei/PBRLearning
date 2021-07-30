#include <iostream>

#include "system/Window.h"

int main()
{
	Window::ptr app = Window::getSingleton();

	app->setWindowSize(1280, 720);
	app->setWindowTitle("PBRLearning");

	app->run();

	return 0;

}