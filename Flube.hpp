#pragma once

#include <cstdlib>

#include "Entity.hpp"

const int FLUBE_WIDTH = 225, FLUBE_HEIGHT = 200;
const int FLUBE_HAT_HEIGHT = 250, FLUBE_HAT_Y_OFFSET = -25;
const std::string TEXTURE_BODY = "img\\body.png", TEXTURE_OVERLAY = "img\\overlay.png";

class Flube : public Entity {
public:
	Flube(std::vector<Argument>& args);

	void Update();
	void Draw();
	Rect Hitbox();
	ENTITY_TYPE GetEntType();

	void Hop();
	void RareHop();
	void Pet();
	void AdvanceFlube();

	int GetFlubeFrame();
	Rect GetFlubeSrcRect();
	Rect GetFlubeDestRect();
	Rect GetFlubeCosmeticSrcRect();
	Rect GetFlubeCosmeticDestRect();

	void FindFood();
	void EatFood();
	void Jump(Vec2 to, float power);
	void Face(int x);

	void DrawCosmetics();
	int DrawCosmetic(const std::string& cvar_cosmetic_fname); // returns 1 if cosmetic found
private:
	float flube_frame = 0.0f,
	      flube_vel_extra = 0.0f,
	      flubber = 0.0f,
	      scale = 0.5f;
	Timer hop_timer, eat_timer;
	bool flip = false,
	     seek_mouse = true, seek_food = false,
	     eating = false;
	int FOOD_ID;
};