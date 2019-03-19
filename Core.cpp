#include "Core.hpp"
#include "Render.hpp"

struct Core Core;

int Core::Init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		LogSDLError();
		return -1;
	}

	win = SDL_CreateWindow("FLUBE :-)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WINDOW_W, WINDOW_H, SDL_WINDOW_OPENGL);
	if (win == nullptr) {
		LogSDLError();
		return -1;
	}

	Render.renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	if (Render.renderer == nullptr) {
		Log("Falling back to software renderer");
		Render.renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_SOFTWARE);
		if (Render.renderer == nullptr) {
			LogSDLError();
			return -1;
		}
	}

	Render.Init();

	SetDefaultVars(Vars);

	Game.Init();

	SetFPSLimit(121);
	SDL_SetRenderDrawBlendMode(Render.renderer, SDL_BLENDMODE_BLEND);

	return 0;
}

void Core::Quit() {
	Render.Quit();
	SDL_DestroyRenderer(Render.renderer);
	SDL_DestroyWindow(win);
	SDL_Quit();
}

void Core::MainLoop() {
	going = true;

	while (going) {
		std::chrono::time_point<std::chrono::high_resolution_clock>
			start = std::chrono::high_resolution_clock::now();

		while (HandleInput());
		MainRender();
		Game.Update();
		CheckGameLog();

		std::chrono::time_point<std::chrono::high_resolution_clock>
			end = std::chrono::high_resolution_clock::now();

		auto delay = delay_time - (end - start);
		if (delay.count() > 0)
			std::this_thread::sleep_for(delay);

		delay_offset = std::chrono::high_resolution_clock::now() - end - delay;

		//end = std::chrono::high_resolution_clock::now();
		if (delay_offset.count() < 0)
			std::this_thread::sleep_for(-delay_offset);

		delay_offset = std::chrono::high_resolution_clock::now() - end - delay;
		if (delay_offset.count() < 0)
			std::this_thread::sleep_for(-delay_offset);

		end = std::chrono::high_resolution_clock::now();
		Game.deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() / 1000.0f;
	}
}

void Core::MainCommand(const std::string & str) {
	auto result = Game.Command(str);
	if (result != "") Log(result);
}

void Core::MainRender() {
	Render.Clear(Game.BG_COLOR);

	/*SDL_Rect map_rect = { 0, 0, MAP_WIDTH, MAP_HEIGHT };
	Game.RenderMap(&map_rect);*/
	Game.RenderAll();

	DrawConsole();
	Render.UpdateRender();
}

int Core::HandleInput() {
	if (!SDL_PollEvent(&event)) return 0;

	switch (event.type) {
	case SDL_MOUSEMOTION:
		HandleMouseMotion();
		break;
	case SDL_MOUSEBUTTONDOWN:
		HandleMouseDown();
		break;
	case SDL_MOUSEBUTTONUP:
		HandleMouseUp();
		break;
	case SDL_KEYDOWN:
		HandleKeyDown();
		break;
	case SDL_KEYUP:
		HandleKeyUp();
		break;
	case SDL_TEXTINPUT:
		HandleTextInput();
		break;
	case SDL_TEXTEDITING:
		HandleTextEditting();
		break;
	case SDL_QUIT:
		going = false;
		break;
	}

	return 1;
}

void Core::HandleKeyDown() {
	// Handle key input when typing into console
	if ((SDL_IsTextInputActive() == SDL_TRUE) && show_full_console) {
		switch (event.key.keysym.sym) {
		case SDLK_BACKSPACE:
			if (!console_input.empty()) {
				if ((cursor >= 0) && (cursor < (signed int)console_input.size())) {
					console_input.erase(cursor, 1);
					--cursor;
				}
			}
			break;
		case SDLK_RETURN:
			Log(console_input);
			MainCommand(console_input);
			console_input = "";
			cursor = -1;
			break;
		case SDLK_LEFT:
			--cursor;
			if (cursor < -1) cursor = -1;
			break;
		case SDLK_RIGHT:
			++cursor;
			if (cursor >= (signed)console_input.size()) cursor = console_input.size() - 1;
			break;
		case SDLK_BACKQUOTE:
			show_full_console = !show_full_console;
			if (show_full_console) {
				SDL_StartTextInput();
				first_letter = true;
			}
			else {
				SDL_StopTextInput();
			}
			break;
		}

		return;
	}

	switch (event.key.keysym.sym) {
	case SDLK_BACKQUOTE:
		show_full_console = !show_full_console;
		if (show_full_console) {
			SDL_StartTextInput();
			first_letter = true;
		}
		else {
			SDL_StopTextInput();
		}
		break;
	}

	//MainCommand("echo \"User pressed key \"" + std::string(SDL_GetScancodeName(SDL_GetScancodeFromKey(event.key.keysym.sym))));
}

void Core::HandleKeyUp() {
	switch (event.key.keysym.sym) {
	/*case SDLK_d:
		MainCommand("-right");
		break;*/
	}
}

void Core::HandleMouseMotion() {
	int x, y;
	SDL_GetMouseState(&x, &y);
	mouse_pos = Vec2(x, y);
}

void Core::HandleTextInput() {
	if (!show_full_console) return;
	if (*event.edit.text < ' ') return;
	if (first_letter) {
		first_letter = false;
		return;
	}

	console_input += *event.edit.text;
	cursor++;
}

void Core::HandleTextEditting() {
	//console_input = event.edit.text;
}

void Core::HandleMouseDown() {
	MainCommand("+mouseclick " + std::to_string(mouse_pos.x) + " " + std::to_string(mouse_pos.y));
}

void Core::HandleMouseUp() {
	MainCommand("-mouseclick " + std::to_string(mouse_pos.x) + " " + std::to_string(mouse_pos.y));
}

void Core::ConsolePrint(const std::string & str) {
	if (Console.size() >= max_lines)
		Console.erase(Console.begin());

	std::string copy = str;
	while (1) {
		auto pos = copy.find_first_of('\n');
		if (pos == std::string::npos) {
			Console.emplace_back(copy, SDL_GetTicks());
			break;
		}
		else {
			if (pos == 0) {
				Console.emplace_back("", SDL_GetTicks());
			} else {
				Console.emplace_back(copy.substr(0, pos), SDL_GetTicks());
			}

			copy.erase(copy.begin(), copy.begin() + pos + 1);
		}
	}

	//Console.emplace_back(str, SDL_GetTicks());
}

void Core::_DrawConciseConsole() {
	if (Console.empty()) return;

	unsigned int curr_tick = SDL_GetTicks();

	bool render = false;
	int i;
	for (i = Console.size() - 1; (i >= 0) && (i > (signed int)Console.size() - (signed int)max_lines_on_screen); --i) {
		if (curr_tick - Console.at(i).time > max_lifespan) break;
		render = true;
	}

	++i;
	if (!render) return;

	
	_DrawConsoleText(i, false, false);
}

void Core::_DrawConsole() {
	int i = (int)Console.size() - max_lines_on_screen;
	if (i < 0) i = 0;

	_DrawConsoleText(i, true, true);
}

void Core::_DrawConsoleText(int i, bool BG, bool INPUT) {
	int y_offset = 1;
	if (BG) {
		int I = i;
		for (; I < (signed int)Console.size(); ++I) {
			auto size = Render.GetTextSize(Console.at(i).str);
			y_offset += 1 + size.y;
		}
		Render.RenderFillRect(Rect(0, 0, WIN_WIDTH, y_offset + 10), { 0x00,0x00,0x00,0xff });
		Render.RenderLine(Vec2(0, y_offset + 10),Vec2(WIN_WIDTH, y_offset + 10), { 0xff,0xff,0xff,0xff });
	}

	y_offset = 1;
	for (; i < (signed int)Console.size(); ++i) {
		Render.RenderText(Vec2(1, y_offset), Console.at(i).str);
		auto size = Render.GetTextSize(Console.at(i).str);
		y_offset += 1 + size.y;
	}

	if (!INPUT) return;

	Render.RenderText(Vec2(1, y_offset), console_input);

	if (SDL_GetTicks() % 1000 < 500) return;
	Render.RenderFillRect(Rect(1 + (1+cursor) * 8, y_offset-1, 8, 10), {0xff,0xff,0xff,0xff});
	if (cursor < (signed)console_input.size()-1) {
		Render.SetColor({0x0,0x0,0x0,0xff});
		Render.RenderChar(Vec2(1 + (cursor+1) * 8, y_offset), console_input.at(cursor+1));
	}
}

void Core::DrawConsole() {
	if (show_full_console)
		_DrawConsole();
	else if (show_console)
		_DrawConciseConsole();
}

void Core::ClearConsole() {
	Console.clear();
}

void Core::SetFPSLimit(int FPS) {
	if (FPS < 24)
		FPS = 24;
	else if (FPS > 289)
		FPS = 289;
	delay_time = std::chrono::nanoseconds(1000000000ll / FPS);
}

void Core::Log(const std::string & str) {
	std::cerr << str << std::endl;
	ConsolePrint(str);
}

void Core::LogError(const std::string & str) {
	std::cerr << "ERROR: " << str << std::endl;
}

void Core::LogSDLError() {
	LogError(std::string(SDL_GetError()));
}

void Core::CheckGameLog() {
	if (Game.game_log.empty()) return;

	for (auto message : Game.game_log) {
		Log(message);
	}
	Game.game_log.clear();
}
