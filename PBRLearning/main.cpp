#include <iostream>

#include "system/PBRenderer.h"

int main()
{
	PBRenderer renderer;

	renderer.setWindowSize(1280, 720);
	renderer.setWindowTitle("PBRLearning");
	renderer.run();

	return 0;
}