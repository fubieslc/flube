#pragma once

#include <SDL.h>
#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <thread>

#include "Render.hpp"
#include "Game.hpp"
#include "Vars.hpp"

#define WINDOW_W 640
#define WINDOW_H 480

struct ConsoleRecord {
	ConsoleRecord(const std::string& _str, unsigned int _time) :
		str(_str), time(_time) { ; }
	unsigned int time;
	std::string str;
};

extern struct Core {
	SDL_Window* win;
	SDL_Event event;

	Vec2 mouse_pos = Vec2(0, 0);
	std::chrono::nanoseconds delay_time = std::chrono::nanoseconds(0);
	void SetFPSLimit(int FPS);

	// set to false to quit main loop
	bool going;

	void Log(const std::string& str);
	void LogError(const std::string& str);
	void LogSDLError();
	void CheckGameLog();

	int Init();
	void Quit();

	int HandleInput();
	void HandleKeyDown();
	void HandleKeyUp();
	void HandleMouseMotion();
	void HandleTextInput();
	void HandleTextEditting();
	void HandleMouseDown();
	void HandleMouseUp();

	void MainCommand(const std::string& str);
	void MainRender();
	void MainLoop();

	void ConsolePrint(const std::string& str);
	void DrawConsole();
	void ClearConsole();

	bool show_console = true, show_full_console = false;
private:
	// Console
	std::vector<ConsoleRecord> Console;
	unsigned int max_lines = 256, max_lines_on_screen = 50, max_lifespan = 2500;

	std::chrono::nanoseconds delay_offset = std::chrono::nanoseconds(0);
	std::string console_input = "";
	int cursor = -1, selection_length = 0;
	bool first_letter = false;

	void _DrawConciseConsole();
	void _DrawConsole();
	void _DrawConsoleText(int i, bool BG, bool INPUT);
} Core;