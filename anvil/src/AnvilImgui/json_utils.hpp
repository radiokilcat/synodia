#pragma once
#include <string>

namespace anvil {

std::string indent(int level) {
	std::string result;
	for (int i = 0; i < level; ++i) {
		result += "\t";
	}
	return result;
}

std::string makeLine(std::string str, int indentLevel) {
	return indent(indentLevel) + str + "\n";
}

}