#pragma once

#include <string>

enum ARG_TYPE { ARG_STRING, ARG_INT, ARG_FLOAT };
struct Argument {
	Argument() { ; }
	Argument(const std::string& _str) : str(_str), arg_type(ARG_STRING) { ; }
	Argument(const int& _int) : INT(_int), arg_type(ARG_INT) { ; }
	Argument(const float& _float) : FLOAT(_float), arg_type(ARG_FLOAT) { ; }

	ARG_TYPE arg_type = ARG_INT;
	std::string str;
	union {
		int INT = 0;
		float FLOAT;
	};

	const std::string ToString() const;
	const int ToInt() const;
	const float ToFloat() const;

	operator int();
	operator float();
	operator std::string();
};