#include "Argument.hpp"
#include "Vars.hpp"

const std::string Argument::ToString() const {
	switch (arg_type) {
	case ARG_STRING:
		return str;
	case ARG_INT:
		return std::to_string(INT);
	case ARG_FLOAT:
		return std::to_string(FLOAT);
	default:
		return "<?>";
	}
}

const int Argument::ToInt() const {
	switch (arg_type) {
	case ARG_INT:
		return INT;
	case ARG_FLOAT:
		return static_cast<int>(FLOAT);
	case ARG_STRING:
		if (!str.empty() && str.at(0) == '$')
			return Vars.Get(str.substr(1), Variable(0));
		else
			return 0;
	}
}

const float Argument::ToFloat() const {
	switch (arg_type) {
	case ARG_INT:
		return static_cast<float>(INT);
	case ARG_FLOAT:
		return FLOAT;
	case ARG_STRING:
		if (!str.empty() && str.at(0) == '$')
			return Vars.Get(str.substr(1), Variable(0.0f));
		else
			return 0.0f;
	}
}

Argument::operator int() {
	return ToInt();
}

Argument::operator float() {
	return ToFloat();
}

Argument::operator std::string() {
	return ToString();
}