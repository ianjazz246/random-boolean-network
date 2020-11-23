#include "network.h"
#include <string>
#include <iostream>

const std::string defaultNetworkPath("networks/basic.txt");

int main(int argc, char** argv) {
	Network net(" ", "0");
	//net.randomizeNetwork(10, 2, 2);

	net.loadFromFile(defaultNetworkPath);

	std::cout << net.exportState();

	std::cout << net;


	return 0;
}