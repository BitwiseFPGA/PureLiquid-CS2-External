#pragma once
#include <string>

namespace Source2 {
	class CUtlSymbolLarge {

	private:

		struct Proxy {
			std::string value;
			const std::string* operator->() const { return &value; }
		};

	private:
		const char* _str;
	public:
		std::string Get() const;

		Proxy operator->() const {
			return Proxy{ Get() };
		};
	};
}