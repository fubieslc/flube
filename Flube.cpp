#include "Flube.hpp"
#include "Food.hpp"

#include "Render.hpp"
#include "Core.hpp"

Flube::Flube(std::vector<Argument>& args) : Entity() {
	hop_timer.Start();
};

void Flube::Update() {
	Gravity();
	FindFood();
	EatFood();
	Hop();
	AdvanceFlube();
	UpdatePos();
	Collide();
}

void Flube::Draw() {
	SDL_Color mod = {0xff,0xff,0xff,0xff};
	mod.r = Vars.Get("flube_r", Variable(0x48)).ToInt();
	mod.g = Vars.Get("flube_g", Variable(0x48)).ToInt();
	mod.b = Vars.Get("flube_b", Variable(0x48)).ToInt();

	SDL_RendererFlip flube_flip = (flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
	Render.RenderTexture(TEXTURE_BODY, &GetFlubeDestRect(), &GetFlubeSrcRect(), 0.0f, flube_flip, mod);
	Render.RenderTexture(TEXTURE_OVERLAY, &GetFlubeDestRect(), &GetFlubeSrcRect(), 0.0f, flube_flip);

	DrawCosmetics();
}

Rect Flube::Hitbox() {
	return Rect(pos.x, pos.y, FLUBE_WIDTH * scale, FLUBE_HEIGHT * scale);
}

ENTITY_TYPE Flube::GetEntType() {
	return ENT_FLUBE;
}

void Flube::Hop() {
	if (!on_ground || !seek_mouse) return;
	if (hop_timer.GetMilliseconds() < 600) return;
	hop_timer.Stop();
	hop_timer.Start();

	if (std::rand() % 12 == 0) RareHop();
	else if (std::rand() % 4 == 0) Jump(Core.mouse_pos, 350.0f);
	else return;

	if (vel.x >= 0.0f) flip = false;
	else flip = true;
}

void Flube::RareHop() {
	vel.y = -450.0f;
	flube_vel_extra = 80.0f;
}

void Flube::Pet() {
	flube_vel_extra += 25.0f;
	flubber = 15.0f * (std::rand() % 2 ? -1.0f : 1.0f);
	hop_timer.Stop();
	hop_timer.Start();
}

void Flube::AdvanceFlube() {
	float flube_vel = Vars.Get("flubevel", Variable(25.0f));

	flube_frame = std::fmod(flube_frame + (flube_vel + flube_vel_extra) * Game.deltaTime, 8.0f);
	flube_vel_extra *= 0.95f;
	flubber *= 0.98f;
	if (flubber >= -0.1f && flubber <= 0.1f) flubber = 0.0f;
}

int Flube::GetFlubeFrame() {
	return (int)flube_frame;
}

Rect Flube::GetFlubeSrcRect() {
	return Rect(GetFlubeFrame() * FLUBE_WIDTH, 0, FLUBE_WIDTH, FLUBE_HEIGHT);
}

Rect Flube::GetFlubeDestRect() {
	auto rect = Hitbox();
	rect.y += (int)(flubber * scale);
	rect.h -= (int)(flubber * scale);
	rect.x -= (int)((flubber / 2.0f) * scale);
	rect.w += (int)((flubber / 2.0f) * scale);
	return rect;
}

Rect Flube::GetFlubeCosmeticSrcRect() {
	return Rect(GetFlubeFrame() * FLUBE_WIDTH, 0, FLUBE_WIDTH, FLUBE_HAT_HEIGHT);
}

Rect Flube::GetFlubeCosmeticDestRect() {
	auto rect = Hitbox();
	rect.x -= (int)(flubber / 2.0f) * scale;
	rect.y += FLUBE_HAT_Y_OFFSET + (int)flubber * scale;
	rect.w += (int)(flubber / 2.0f) * scale;
	rect.h = FLUBE_HAT_HEIGHT - (int)flubber * scale;
	return rect;
}

void Flube::FindFood() {
	for (auto ent = Entities.Ents.begin(); ent != Entities.Ents.end(); ++ent) {
		auto food = dynamic_cast<Food*>(ent->get());
		if (food == nullptr) continue;

		if (!on_ground || food->IsHeld() || eating) return;
		seek_mouse = false;
		seek_food = true;
		FOOD_ID = food->ID;

		auto h = Hitbox();
		if (Dist(food->pos - (pos + Vec2_F(h.w/2, h.h - 50))) < 100.0f) {
			eating = true;
			eat_timer.Stop();
			eat_timer.Start();
			return;
		}

		Jump((Vec2)food->pos - Vec2(0, 100), 250.0f);
		Face(food->pos.x);
		return;
	}

	seek_food = false;
	seek_mouse = true;
}

void Flube::EatFood() {
	if (!eating) return;

	for (auto ent = Entities.Ents.begin(); ent != Entities.Ents.end(); ++ent) {
		if ((*ent)->ID != FOOD_ID) continue;
		
		if (eat_timer.GetMilliseconds() > 2300) {
			eating = false;
			seek_mouse = true;
			seek_food = false;
			RareHop();
			scale += 0.1f;
			if (scale > 1.5f) scale = 1.5f;
			(*ent)->Delete();
		}

		Vec2 food_pos = (Vec2)pos + (Vec2(116.0f, 103.0f) * scale);
		food_pos.y += std::sin(eat_timer.GetSeconds() * 5.0f) * 10.0f;
		(*ent)->Goto(food_pos);
		(*ent)->vel = Vec2_F(0.0f,0.0f);
	}
}

void Flube::Jump(Vec2 to, float power) {
	Vec2_F dif = Vec2_F(to.x, to.y) - (pos + Vec2_F(FLUBE_WIDTH/2.0f, FLUBE_HEIGHT/2.0f));
	if (dif.x == 0.0f && dif.y == 0.0f) return;
	dif = (dif / Dist(dif)) * power;
	vel = vel + dif;
}

void Flube::Face(int x) {
	if (x < pos.x + Hitbox().w / 2) flip = true;
	else flip = false;
}

void Flube::DrawCosmetics() {
	int count = 0;
	while (1) {
		std::string str = "cosmetic" + std::to_string(count);
		if (!DrawCosmetic(str)) return;
		++count;
	}
}

int Flube::DrawCosmetic(const std::string& cvar_cosmetic_fname) {
	std::string hat_file = Vars.Get(cvar_cosmetic_fname, Variable(""));
	if (!hat_file.empty()) {
		SDL_RendererFlip flube_flip = (flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
		Render.RenderTexture(hat_file, &GetFlubeCosmeticDestRect(),
			&GetFlubeCosmeticSrcRect(), 0.0f, flube_flip);
		return 1;
	}

	return 0;
}
