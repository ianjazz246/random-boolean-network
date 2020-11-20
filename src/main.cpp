#include <iostream>
#include "network.h"

int main(int argc, char** argv) {
	std::cout << "Hello?\n";
	Network net("f", "0");
	net.randomizeNetwork(10, 2, 2);

	net.loadStateFromFile("networks/test.txt");

	std::cout << net;

	return 0;
}