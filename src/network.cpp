#include "network.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <random>
#include <algorithm>

// Character that follows the node index, before the node state/connections
const char nodeIDelim = ':';
// Character to use, repeated, as a divider between the states and connections
const char sectSepChar = '-';
const unsigned int sectSepCharRepeat = 10;
const char stateDelim = ',';

bool defaultTransformer(const Node& node, const std::vector<Node>& nodes) {
	bool state = node.state;
	for (const auto& otherNodeI : node.connectedNodes) {
		state &= nodes[otherNodeI].state;
	}
	return state;
}

Network::Network(bool (*transformer)(const Node&, const std::vector<Node>&),
	std::string onString, std::string offString) :
	transformer(transformer), onString(onString), offString(offString)
	{}

Network::Network(std::string onString, std::string offString) :
	transformer(defaultTransformer), onString(onString), offString(offString)
	{}

// For now, minConnections is ignored. The network will always have maxConnections connections
void Network::randomizeNetwork(unsigned int numNodes, unsigned int minConnections, unsigned int maxConnections) {
	if (maxConnections > numNodes) {
		throw std::invalid_argument("maxConnections greater than number");
	}
	if (minConnections > maxConnections) {
		throw std::invalid_argument("minConnections must be less than maxConnections");
	}

	std::random_device rd;
	std::mt19937 rng(rd());
	std::uniform_int_distribution<std::mt19937::result_type> boolDist(0, 1);
	std::uniform_int_distribution<std::mt19937::result_type> connectionDist(0, numNodes-1);
	std::uniform_int_distribution<std::mt19937::result_type> numConnectionsDist(minConnections, maxConnections);

	std::vector<Node> tempNodes(numNodes);

	std::vector<unsigned int> possibleConnections(numNodes);
	for (unsigned int i = 0; i < possibleConnections.size(); ++i) {
		possibleConnections[i] = i;
	}

	for (auto& node : tempNodes) {
		node.state = boolDist(rng) == 1;
		node.connectedNodes = std::vector<unsigned int>(maxConnections);	
		
		// Fisher-Yates algorithm
		for (unsigned int i = 0; i < maxConnections; ++i) {
			std::uniform_int_distribution<std::mt19937::result_type> randConnDist(i, numNodes);
			unsigned int randIndex = randConnDist(rng);
			node.connectedNodes.push_back(possibleConnections[randIndex]);
			// Swap selected element into "struck out position", taking element there into pool
			std::iter_swap(possibleConnections.begin() + randIndex, possibleConnections.begin() + i);
		}
	}

	using std::swap;

	swap(nodes, tempNodes);
}

void Network::loadFromFile(const std::string& path) {
	std::ifstream inFile;
	inFile.open(path);
	if (inFile.fail()) {
		throw std::logic_error("Error opening file");
	}
	int numNodes;
	inFile >> numNodes;
	std::vector<Node> newNodes(numNodes);
	
	// Read states
	for (int i = 0; i < numNodes; ++i) {
		int num;
		char assignChar;
		inFile >> num;
		// File starts counting from 1
		if ((num - 1) != i) {
			throw std::logic_error("Error reading initial states");
		}
		inFile >> assignChar;
		if (assignChar != nodeIDelim) {
			throw std::logic_error("Error reading initial states");
		}
		inFile >> num;
		if (!inFile.good()) {
			throw std::logic_error("Error reading initial states");
		}
		newNodes[i].state = (num == 1);
	}

	std::string stateConnBorder;
	do {
		std::getline(inFile, stateConnBorder);
	} while (stateConnBorder == "");
	if (stateConnBorder[0] != sectSepChar) {
		throw std::logic_error("Error reading file");
	}
	

	// Read connections
	for (int i = 0; i < numNodes; ++i) {
		int num;
		char assignChar;
		char delim;

		inFile >> num;
		// File starts counting from 1
		if ((num - 1) != i) {
			throw std::logic_error("Error reading initial states");
		}
		inFile >> assignChar;
		if (assignChar != nodeIDelim) {
			throw std::logic_error("Error reading initial states");
		}
		while (true) {
			inFile >> num;
			newNodes[i].connectedNodes.push_back(num);
			inFile >> delim;
			if (inFile.eof()) {
				// Reached end of file
				break;
			}
			if (delim != stateDelim) {
				inFile.putback(delim);
				break;
			}
		}
	}

	using std::swap;
	swap(newNodes, nodes);

	inFile.close();
}

std::string Network::exportState() const {
	std::string output;
	output.append(std::to_string(nodes.size()));
	output.push_back('\n');
	for (size_t i = 0; i < nodes.size(); ++i) {
		output.append(std::to_string(i + 1));
		output.push_back(nodeIDelim);
		output.push_back(' ');
		output.push_back(nodes[i].state ? '1' : '0');
		output.push_back('\n');
	}
	// Divider between states and connections
	std::string stateConnSectSep(sectSepCharRepeat, sectSepChar);
	output.append(stateConnSectSep);
	output.push_back('\n');
	// Connections
	for (size_t i = 0; i < nodes.size(); ++i) {
		const std::vector<unsigned int>& connectedNodes = nodes[i].connectedNodes;
		output.append(std::to_string(i + 1));
		output.push_back(nodeIDelim);
		output.push_back(' ');
		for (size_t j = 0; j < connectedNodes.size(); ++j) {
			output.append(std::to_string(connectedNodes[j]));
			if (j != connectedNodes.size() - 1) {
				output.push_back(stateDelim);
				output.push_back(' ');
			}
		}
		output.push_back('\n');
	}

	return output;
}

void Network::exportToFile(const std::string& path) const {
	std::ofstream outFile;

	outFile.open(path);

	if (outFile.fail()) {
		throw std::logic_error("Error opening file");
	}

	outFile << exportState();
}

void Network::step() {
	// To be passed to transformer function
	for (auto& node : nodes) {
		node.state = transformer(node, nodes);
	}
}

std::ostream& operator << (std::ostream& out, const Network& network) {
	for (const auto& node : network.nodes) {
		out << (node.state ? network.onString : network.offString);
		// if (node.state) {
		// 	out << network.onString;
		// } else {
		// 	out << network.offString;
		// }
	}
	return out;
}