#include "configuration.h"

namespace anvil {
	void Configuration::load(std::string path) 
	{
		std::cout << path;
		std::ifstream is;
		is.open(path);
		std::string test;
		is >> test;
		std::cout << test;
	}
}