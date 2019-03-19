#pragma once

#include "Entity.hpp"
#include "Timer.hpp"

const int FOOD_WIDTH = 64, FOOD_HEIGHT = 55;
const std::string TEXTURE_FOOD = "img\\berries.png";

class Food : public Entity {
public:
	Food(std::vector<Argument>& args);

	void Collide();
	void Update();
	void Draw();
	Rect Hitbox();
	ENTITY_TYPE GetEntType();

	void Drop();
	bool IsHeld();
private:
	bool held = true;
};