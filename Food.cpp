#include "Food.hpp"
#include "Vars.hpp"

Food::Food(std::vector<Argument>& args) {
	if (args.size() < 2) return;
	if (args.at(0).arg_type == ARG_STRING || args.at(1).arg_type == ARG_STRING) return;
	pos = Vec2_F((float)args.at(0), (float)args.at(1));
}

void Food::Collide() {
	on_left_wall = false;
	on_right_wall = false;
	on_ground = false;
	on_ceiling = false;

	auto hitbox = Hitbox();

	if (pos.x < 66) {
		pos.x = 66;
		vel.x = 0;
		on_left_wall = true;
	} else if (pos.x + hitbox.w > 574) {
		pos.x = 574 - hitbox.w;
		vel.x = 0;
		on_right_wall = true;
	}

	int floor_offset = Vars.Get("floor_offset", Variable(0));
	if (pos.y < 0) {
		pos.y = 0;
		vel.y = 0;
		on_ceiling = true;
	} else if (pos.y + hitbox.h > 480 - floor_offset) {
		pos.y = 480 - hitbox.h - floor_offset;
		vel.y = 0;
		on_ground = true;

		// friction
		vel.x *= (float)Vars.Get("friction", Variable(0.96f));
	}
}

void Food::Update() {
	if (held) {
		GotoMouse();
		Collide();
	} else {
		Gravity();
		UpdatePos();
		Collide();
	}
}

void Food::Draw() {
	Render.RenderTexture(TEXTURE_FOOD, &Hitbox(), nullptr, 0.0f, SDL_FLIP_NONE,
		{(Uint8)0xff,(Uint8)0xff,(Uint8)0xff, (Uint8)(held ? 0xA0 : 0xff)});
}

Rect Food::Hitbox() {
	return Rect(pos.x, pos.y, FOOD_WIDTH, FOOD_HEIGHT);
}

ENTITY_TYPE Food::GetEntType() {
	return ENT_FOOD;
}

void Food::Drop() {
	held = false;
}

bool Food::IsHeld() {
	return held;
}
