#pragma once

#include <map>

#include "Argument.hpp"

using Variable = Argument;
//struct Variable : public Argument { };

extern struct Vars {
	Variable* GetVar(const std::string& str);
	void SetVar(const std::string& str, const Variable& var);

	Variable Get(const std::string& str, Variable def);

	std::map<std::string, Variable> vars;
} Vars;

void SetDefaultVars(struct Vars&);