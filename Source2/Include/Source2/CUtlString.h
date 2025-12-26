#pragma once
#include <GlobalData/Include.h>
#include <string>
namespace Source2 {
	class CUtlString {
	private:
		const char* _str;
	public:
		std::string Get() const;
	};
}

/*
namespace Source2 {
	class CUtlString {
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
*/