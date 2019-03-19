#include "HUD.hpp"

#include "Core.hpp"
#include "Game.hpp"

void HUD::Update() {
	for (auto element = elements.begin(); element != elements.end(); ++element) {
		(*element)->Update();
	}
}

void HUD::Draw() {
	for (auto element = elements.begin(); element != elements.end(); ++element) {
		(*element)->Draw();
	}
}

void HUD::Click() {
	for (auto element = elements.begin(); element != elements.end(); ++element) {
		if ((*element)->hovered)
			(*element)->Click();
	}
}

void Button::Update() {
	if (RectPointCollision(Rect(pos.x, pos.y, size.x, size.y), Core.mouse_pos))
		hovered = true;
	else
		hovered = false;
}

void Button::Draw() {
	Rect rect = Rect(pos.x, pos.y, size.x, size.y);
	SDL_Color c;
	if (hovered)
		c = { 0xe0, 0xe0, 0xe0, 0xff };
	else
		c = { 0xff, 0xff, 0xff, 0xff };

	Render.RenderTexture(img, &rect, nullptr, 0.0f, SDL_FLIP_NONE, c);
}

void Food_Button::Click() {
	Game.Command("entcreate $ENT_FOOD");
}
