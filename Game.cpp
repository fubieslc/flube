#include "Game.hpp"
#include "Core.hpp"
#include "Entity.hpp"
#include "Vars.hpp"

struct Game Game;

void Game::Init() {
	commands["echo"] = &_echo;
	commands["quit"] = &_quit;
	commands["clear"] = &_clear;
	commands["showfps"] = &_showfps;
	commands["showconsole"] = &_showconsole;
	commands["printvars"] = &_printvars;
	commands["entcreate"] = &_entcreate;

	commands["+mouseclick"] = &_mouseclick;
	commands["-mouseclick"] = &_mousedrop;
		
	commands["setvar"] = &_setvar;
	commands["setfps"] = &_setfps;

	Entities.AddEntity(ENT_FLUBE);

	GameHud.elements.push_back(std::make_unique<Food_Button>(Vec2(20,50), Vec2(64,55), TEXTURE_FOOD));
}

void Game::Quit() {

}

void Game::Update() {
	Entities.UpdateEntities();
	GameHud.Update();
}

void Game::RenderAll() {
	Render.RenderTexture(Vars.Get("bg_img",Variable("img\\background.png")), nullptr, nullptr);
	Entities.RenderEntities();
	GameHud.Draw();
	if (show_fps) DrawFPS();
}

std::string Game::Command(const std::string & command) {
	// First, split command into arguments
	std::vector<Argument> args;
	std::stringstream ss;
	ss.str(command);

	char c = ss.get();
	while (!ss.eof()) {
		std::string str = "";

		int char_count = 0;
		bool quote_mark = false,
		     is_numeric = true,
		     is_decimal = false;

		// parse one argument loop
		while (1) {
			if (c < ' ') { // ignore control characters, break on EOF
				if (ss.eof()) break;
				c = ss.get();
				continue;
			}
			else if (c == '"') {
				if (quote_mark) {
					c = ss.get();
					// get rid of whitespaces after quote mark
					while (std::isspace(c)) c = ss.get();
					break;
				}
				else quote_mark = true;
				c = ss.get();
			}
			else if (std::isspace(c) && !quote_mark) {
				// ignore whitespaces
				while (std::isspace(c)) c = ss.get();
				break;
			}
			else {
				// checks to see if argument is a number
				if (c == '-') {
					// only first character can be a negative sign in a number
					if (char_count != 0)
						is_numeric = false;
				} else if (c == '.') {
					// set numeric to false, two decimal points is a malformed number
					if (is_decimal) is_numeric = false;
					else is_decimal = true;
				}
				else if (c < '0' || c > '9') {
					is_numeric = false;
				}

				str += c;
				c = ss.get();
				++char_count;
			}
		}

		if (is_numeric) {
			if (is_decimal) {
				try {
					args.emplace_back(std::stof(str));
				} catch (...) {
					args.emplace_back(str);
				}
			} else {
				try {
					args.emplace_back(std::stoi(str));
				} catch (...) {
					args.emplace_back(str);
				}
			}
		} else {
			args.emplace_back(str);
		}
	}

	// if arguments is empty or first argument is not a string, return error
	if (args.empty() || args.front().arg_type != ARG_STRING) {
		command_found = false;

		if (args.empty())
			return "";
	}

	std::string comm = args.front().str;
	if (!CommandExists(comm)) {
		command_found = false;
		return "Command \"" + comm + "\" not found";
	}

	command_found = true;
	return commands[comm](args); // Calls command
}

bool Game::CommandFound() {
	return command_found;
}

bool Game::CommandExists(const std::string & command) {
	auto comm = commands.find(command);
	if (comm == commands.end())
		return false;
	return true;
}

void Game::DrawFPS() {
	FPS = (static_cast<int>(1.0f / deltaTime) + FPS) / 2;
	Render.RenderText(Vec2(1, WIN_HEIGHT - 9), std::to_string(FPS), 1, {0xff,0xff,0x00,0xff});
}

void _PetFlube(Flube* flube) {
	if (RectPointCollision(flube->Hitbox(), Core.mouse_pos))
		flube->Pet();
}

void Game::PetFlube(Vec2 vec) {
	Entities.ApplyFunc<Flube>(_PetFlube);
}

void _DropHeld(Food* food) {
	food->Drop();
}

void Game::DropHeld(Vec2 vec) {
	Entities.ApplyFunc<Food>(_DropHeld);
}

void Game::Log(const std::string & str) {
	// limit log size
	if (game_log.size() >= 512) return;
	game_log.push_back(str);
}

void Game::HandleMouseDown(Vec2 vec) {
	PetFlube(vec);
	GameHud.Click();
}

void Game::HandleMouseUp(Vec2 vec) {
	DropHeld(vec);
}


// Command functions
// 
// static std::string _COMMAND( const std::vector<Argument>& args );
//
//

bool IS_NUMBER(const Argument& arg) {
	if (arg.arg_type == ARG_STRING) {
		if (arg.str.empty() || arg.str.at(0) != '$')
			return false;

		auto var = Vars.GetVar(arg.str.substr(1));
		if (var == nullptr) return false;
		if (var->arg_type != ARG_STRING) return true;
		return false;
	}
	else {
		return true;
	}
}

bool IS_STRING(const Argument& arg) {
	return !IS_NUMBER(arg);
}

std::string _echo(const std::vector<Argument>& args) {
	if (args.size() <= 1) return "\0";
	else {
		std::string result = "";
		for (auto arg = args.begin() + 1; arg != args.end(); ++arg) {
			std::string arg_str = arg->ToString();
			try {
				if (arg_str.at(0) == '$') {
					arg_str.erase(arg_str.begin());
					auto it = Vars.GetVar(arg_str);
					if (it == nullptr) {
						arg_str = "<?>";
					}
					else {
						arg_str = it->ToString();
					}
				}
			}
			catch (...) { ; }

			result += arg_str;
		}

		return result;
	}
}

std::string _quit(const std::vector<Argument>& args) {
	Core.going = false; 
	return std::string();
}

static std::string _clear(const std::vector<Argument>& args) {
	Core.ClearConsole();
	return std::string();
}

std::string _showfps(const std::vector<Argument>& args) {
	Game.show_fps = !Game.show_fps;
	return std::string();
}

std::string _showconsole(const std::vector<Argument>& args) {
	Core.show_console = !Core.show_console;
	return std::string();
}

std::string _mouseclick(const std::vector<Argument>& args) {
	const std::string USE_MESSAGE = "+mouseclick - x y";
	int x, y;
	if (args.size() < 3) {
		return USE_MESSAGE;
	}

	if (IS_STRING(args[1]) || IS_STRING(args[2])) {
		return USE_MESSAGE;
	}

	x = args.at(1).ToInt();
	y = args.at(2).ToInt();

	Game.HandleMouseDown(Vec2(x,y));

	return std::string();
}

std::string _mousedrop(const std::vector<Argument>& args) {
	const std::string USE_MESSAGE = "-mouseclick - x y";
	int x, y;
	if (args.size() < 3) {
		return USE_MESSAGE;
	}

	if (IS_STRING(args[1]) || IS_STRING(args[2])) {
		return USE_MESSAGE;
	}

	x = args.at(1).ToInt();
	y = args.at(2).ToInt();

	Game.HandleMouseUp(Vec2(x,y));

	return std::string();
}

std::string _entcreate(const std::vector<Argument>& args) {
	const std::string USE_MESSAGE = "entcreate - ent_id args...";
	if (args.size() < 2 || IS_STRING(args[1])) {
		return USE_MESSAGE;
	}

	auto ent = Entities.AddEntity((ENTITY_TYPE)args.at(1).ToInt());
	if (ent == nullptr) {
		return USE_MESSAGE;
	}

	return std::string();
}

std::string _setvar(const std::vector<Argument>& args) {
	const std::string USE_MESSAGE = "setvar - var value";
	std::string var;
	if (args.size() < 3) {
		return USE_MESSAGE;
	}

	if (IS_NUMBER(args[1]))
		return USE_MESSAGE;

	var = args.at(1).ToString();

	Vars.SetVar(var, args.at(2));

	return std::string();
}

std::string _setfps(const std::vector<Argument>& args) {
	const std::string USE_MESSAGE = "setfps - fps";
	int fps;
	if (args.size() < 2) {
		return USE_MESSAGE;
	}

	if (IS_STRING(args[1])) {
		return USE_MESSAGE;
	}

	fps = args.at(1).ToInt();
	Core.SetFPSLimit(fps);

	return std::string();
}

std::string _printvars(const std::vector<Argument>& args) {
	std::string result = "";

	bool first = true;
	for (auto var = Vars.vars.begin(); var != Vars.vars.end(); ++var) {
		if (first) first = !first;
		else result += '\n';

		std::string line = var->first + " = " + (std::string)var->second;
		result += line;
	}

	return result;
}
