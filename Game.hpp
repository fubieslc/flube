#pragma once

#include "Render.hpp"
#include "Entity.hpp"
#include "HUD.hpp"

#include <vector>
#include <array>
#include <string>
#include <iostream>
#include <sstream>
#include <map>
#include <memory>
#include <cctype>

static const int WIN_WIDTH = 640,
	WIN_HEIGHT = 480;

extern struct Game {
	void Init();
	void Quit();

	void Update();
	void RenderAll();

	HUD GameHud;

	// Issue a command.
	// If command doesn't exist, command_found is set to false
	// otherwise it should be true
	// Returns a message from the command, if any.
	std::string Command(const std::string& command);
	bool CommandFound();

	SDL_Color BG_COLOR = { 0xBB , 0xBB , 0xBB , 0xFF };

	std::vector<std::string> game_log;
	void Log(const std::string& str);

	void HandleMouseDown(Vec2);
	void HandleMouseUp(Vec2);

	bool show_fps = true;
	void DrawFPS();
	int FPS = 0;

	void PetFlube(Vec2);
	void DropHeld(Vec2);

	float deltaTime = 0.016666f;
private:
	bool command_found = false;

	// Map of commands to functions
	std::map<std::string, std::string (*)( const std::vector<Argument>& )> commands;
	bool CommandExists(const std::string& command);
} Game;

// Command functions
// 
// static std::string COMMAND( const std::vector<Argument>& args );
//

static std::string _echo( const std::vector<Argument>& args );
static std::string _quit( const std::vector<Argument>& args );
static std::string _clear(const std::vector<Argument>& args);
static std::string _showfps(const std::vector<Argument>& args);
static std::string _showconsole(const std::vector<Argument>& args);
static std::string _mouseclick(const std::vector<Argument>& args);
static std::string _mousedrop(const std::vector<Argument>& args);
static std::string _entcreate(const std::vector<Argument>& args);

static std::string _setvar(const std::vector<Argument>& args);
static std::string _setfps(const std::vector<Argument>& args);
static std::string _printvars(const std::vector<Argument>& args);