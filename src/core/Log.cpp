#include "core/Log.h"
#include <iostream>

namespace dd {
	void LogInfo(const std::string& s) { std::cout << "[info] " << s << "\n"; }
	void LogError(const std::string& s) { std::cerr << "[error] " << s << "\n"; }
}
