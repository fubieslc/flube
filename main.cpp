// main.cpp
//

#include "Core.hpp"

int main(int argc, char** argv) {
	if (Core.Init()) {
		Core.Quit();
		return -1;
	}
	else {
		Core.MainLoop();
		Core.Quit();
		return 0;
	}

	return 0;
}