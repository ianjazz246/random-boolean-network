#include "network.h"
#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <random>
#include <algorithm>

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

	std::swap(nodes, tempNodes);
}

void Network::loadStateFromFile(const std::string& path) {
	
	std::ifstream inFile;
	inFile.open(path);
	if (inFile.is_open()) {
		int numNodes;
		inFile >> numNodes;
	}


	inFile.close();
}

void Network::exportStateToFile(const std::string& path) const{
	// TODO
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