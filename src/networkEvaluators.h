#pragma once
#include "node.h"
#include <functional>
#include <unordered_map>

namespace NetworkEvaluators {
	bool XorEvaluator(const Node& node, const std::vector<Node>& allNodes) {
		bool state = node.state;
		for (const auto& connI : node.connectedNodes) {
			state ^= allNodes[connI].state;
		}
		return state;
	}
	bool AndEvaluator(const Node& node, const std::vector<Node>& allNodes) {
		bool state = node.state;
		for (const auto& connI : node.connectedNodes) {
			state &= allNodes[connI].state;
		}
		return state;
	}
	bool OrEvaluator(const Node& node, const std::vector<Node>& allNodes) {
		bool state = node.state;
		for (const auto& connI : node.connectedNodes) {
			state |= allNodes[connI].state;
		}
		return state;
	}

	const std::unordered_map<
		std::string, bool (*)(const Node&,const std::vector<Node>&)
	> evaluators = {
		{"xor", NetworkEvaluators::XorEvaluator},
		{"and", NetworkEvaluators::AndEvaluator},
		{"or", NetworkEvaluators::OrEvaluator}
	};

	const std::string defaultEvaluatorName = "xor";
}