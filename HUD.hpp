#pragma once

#include <memory>

#include "Vars.hpp"
#include "Render.hpp"
#include "Hitreg.hpp"

struct HUD_Element {
	HUD_Element(Vec2 _pos) : pos(_pos) { ; }

	virtual void Update() = 0;
	virtual void Click() = 0;
	virtual void Draw() = 0;

	Vec2 pos;
	bool hovered = false;
};

struct Button : public HUD_Element {
	Button(Vec2 _pos, Vec2 _size, const std::string& _img) : HUD_Element(_pos), size(_size), img(_img)
		{ ; }

	std::string img;
	Vec2 size;

	void Update();
	void Draw();
};

struct Food_Button : public Button {
	Food_Button(Vec2 _pos, Vec2 _size, const std::string& _img) : Button(_pos, _size, _img) { ; }

	void Click();
};

class HUD {
public:
	void Update();
	void Draw();
	void Click();

	std::vector<std::unique_ptr<HUD_Element>> elements;
};