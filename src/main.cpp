#include "network.h"
#include <string>
#include <iostream>
#include <exception>

const std::string defaultNetworkPath("networks/basic.txt");

int main(int argc, char** argv) {
	Network net(" ", "0");
	//net.randomizeNetwork(10, 2, 2);

	try {
		std::cout << "Loading network state from file" << argv[1] << '\n';
		if (argc > 1) {
			std::cout << argv[1] << '\n';
			net.loadFromFile(argv[1]);
		} else {
			net.randomizeNetwork(10, 1, 1);
		}
	} catch(const std::runtime_error& e) {
		std::cerr << e.what();
		return 1;
	}
	std::cout << net.exportState();

	char inChar;

	std::cout << "Hit enter to step through network. Enter q to quit\n";

	while (true) {
		std::cin.get(inChar);
		if (inChar != 'q') {
			net.step();
			std::cout << net;
		} else {
			return 0;
		}
	}

	return 0;
}