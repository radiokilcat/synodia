#include "configuration.h"
#include "inipp.h"

namespace anvil {
	void Configuration::load(std::string path) 
	{
		inipp::Ini<char> ini;
		std::ifstream is(path);
		ini.parse(is);
		std::cout << "raw ini file:" << std::endl;
		ini.generate(std::cout);
		ini.strip_trailing_comments();
		ini.default_section(ini.sections["Window"]);
		ini.interpolate();
		std::cout << "ini file after default section and interpolation:" << std::endl;
		ini.generate(std::cout);
	}
}