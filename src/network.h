#pragma once
#include "node.h"
#include <iostream>

class Network {
   public:
    void loadFromFile(const std::string& path);
		void exportToFile(const std::string& path) const;
		std::string exportState() const;
		void randomizeNetwork(unsigned int numNodes, unsigned int minConnections, unsigned int maxConnections);
    void step();
		friend std::ostream& operator << (std::ostream& out, const Network& network);

		// evalName: name of evaluator function, as assigned in NetworkEvaluators::evaluators
		Network(std::string evalName,
			std::string onString, std::string offString
		);

		Network(std::string onString, std::string offString);

   private:
	 	// May be more memory efficient to have used one std::vector<bool>
		// for the node states and to use a 2nd vector to store the connections
		// because vector can use 1 bit per bool, but I don't think struct can.
    std::vector<Node> nodes;

		std::string evaluatorName;

		std::string onString;
		std::string offString;
};