#include "network.h"
#include <string>
#include <map>
#include <iostream>
#include <exception>

const std::string defaultNetworkPath("networks/basic.txt");

void printHelp() {
	std::cout << "Usage: main.exe [options] network-file\n";
	std::cout << "If no options are passed, the network is read from network-file\n";
	std::cout << "If options are passed, a random network will be generated, with the\n";
	std::cout << "following the parameters.\n";
	std::cout << "-n number-of-nodes\n";
	std::cout << "-c number-of-connections\n";

	std::cout << std::flush;
}

// Returns the value of the -arg at index i in argv after that arg
// If argv is {"programName", "-arg1", "Potato"}
// getArgValue(argc, argv, 1) returns "Potato"
char* getArgValue(const int& argc, char* argv[], const int i) {
	// Bounds check
	if (i < argc - 1) {
		return argv[i+1];
	} else {
		throw std::runtime_error("Error reading arguments");
	}
}

struct argsStruct {
	char* fileName = nullptr;
	bool help = 0;
	int numNodes = 0;
	int numConnections = 0;
};

argsStruct parseArguments(int argc, char* argv[]) {
	argsStruct args;
	for (int i = 1; i < argc; ++i) {
		std::cout << argv[i];
		auto argStrLen = strlen(argv[i]);
		// Check if argument is a "-" argument, which is at least 2 characters and starts with "-"
		if (argStrLen > 1 && argv[i][0] == '-') {
			switch (argv[i][1]) {
				case 'h':
					args.help = true;
					return args;
				case 'n':
					args.numNodes = std::atoi(getArgValue(argc, argv, i));
					++i;
					break;
				case 'c':
					args.numConnections = std::atoi(getArgValue(argc, argv, i));
					++i;
					break;
				default:
					throw std::runtime_error("Unknown argument");
					break;
			}
		} else {
			// Non "-"" argument
			// Read as filename to read network from
			args.fileName = argv[i];
		}
	}
	return args;
}

int main(int argc, char* argv[]) {
	argsStruct args;
	try {
		args = parseArguments(argc, argv);
	} catch (std::runtime_error& e) {
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Help: " << args.help << std::endl;
	std::cout << "# Nodes: " << args.numNodes << std::endl;
	std::cout << "# Connections: " << args.numConnections << std::endl;
	if (args.fileName) {
		std::cout << "Filename: " << args.fileName << std::endl;
	}

	if (args.help) {
		printHelp();
		return 0;
	}

	Network net(" ", "0");
	//net.randomizeNetwork(10, 2, 2);

	try {
		if (args.fileName) {
			std::cout << "Loading network state from file: " << args.fileName << '\n';
			net.loadFromFile(args.fileName);
		} else {
			std::cout << "Creating randomized network" << '\n';
			// Create network with 10 nodes, 1 connection each node
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