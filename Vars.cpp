#include "Vars.hpp"

struct Vars Vars;

using Variable = Argument;

Variable* Vars::GetVar(const std::string & str) {
	// TODO: insert return statement here
	auto var = vars.find(str);
	if (var == vars.end())
		return nullptr;
	else
		return &(var->second);
}

void Vars::SetVar(const std::string & str, const Variable& var) {
	vars[str] = var;
}

Variable Vars::Get(const std::string & str, Variable def) {
	auto var = GetVar(str);
	if (var && var->arg_type == def.arg_type) return *var;
	return def;
}

void SetDefaultVars(struct Vars & var) {
	var.SetVar("gravity", Variable(600.0f));
	var.SetVar("friction", Variable(0.96f));

	var.SetVar("flubevel", Variable(19.0f));
	var.SetVar("flube_r", Variable(0x48));
	var.SetVar("flube_g", Variable(0x48));
	var.SetVar("flube_b", Variable(0x48));

	var.SetVar("floor_offset", Variable(66));
	var.SetVar("bg_img", Variable("img\\background.png"));
	var.SetVar("hat_img", Variable(""));

	var.SetVar("ENT_FLUBE", Variable(0));
	var.SetVar("ENT_FOOD", Variable(1));
}
