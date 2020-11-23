#include <string>
#include <vector>
#include <iostream>

struct Node {
		bool state;
    std::vector<unsigned int> connectedNodes;
};

class Network {
   public:
    void loadFromFile(const std::string& path);
		void exportToFile(const std::string& path) const;
		std::string exportState() const;
		void randomizeNetwork(unsigned int numNodes, unsigned int minConnections, unsigned int maxConnections);
    void step();
		friend std::ostream& operator << (std::ostream& out, const Network& network);

		Network(bool (*transformer)(const Node&, const std::vector<Node>&),
			std::string onString, std::string offString
		);

		Network(std::string onString, std::string offString);



   private:
	 	// May be more memory efficient to have used one std::vector<bool>
		// for the node states and to use a 2nd vector to store the connections
		// because vector can use 1 bit per bool, but I don't think struct can.
    std::vector<Node> nodes;

		// Function is passed the value of the connected nodes.
		// It should return the new state of the node given the first.
		// Pointer called transformer to a function that returns bool
    // and has one parameter: const std::vector<unsigned int>&
    bool (*transformer)(const Node&, const std::vector<Node>&);

		std::string onString;
		std::string offString;
};