#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "Argument.hpp"
#include "Hitreg.hpp"
#include "Render.hpp"
#include "Timer.hpp"

// Entity base class

enum ENTITY_TYPE { ENT_FLUBE , ENT_FOOD };

static long long ID_COUNTER = 0;
class Entity {
public:
	Entity();

	long long ID = 0;

	Vec2_F pos = Vec2_F(320.0f, 240.0f), vel = Vec2_F(0.0f,0.0f);
	bool on_ground = false,
	     on_ceiling = false,
	     on_left_wall = false,
	     on_right_wall = false;

	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual Rect Hitbox() = 0;
	virtual ENTITY_TYPE GetEntType() = 0;

	virtual void Collide();

	void UpdatePos();
	void Gravity();
	void Goto(Vec2 pos);
	void GotoMouse();

	void Delete();
	bool ToDelete();
private:
	// set to true to delete entity
	bool delete_flag = false;
};

extern struct Entities {
	Entity* AddEntity(const ENTITY_TYPE, std::vector<Argument>& args);
	Entity* AddEntity(const ENTITY_TYPE);

	void UpdateEntities();
	void RenderEntities();

	std::vector< std::unique_ptr<Entity> > Ents;

	template <class type>
	void ApplyFunc(void (*func)(type*));
private:
	std::unique_ptr<Entity> CreateEntity(const ENTITY_TYPE ent, std::vector<Argument>& args);
	void CleanEntities();
} Entities;

#include "Flube.hpp"
#include "Food.hpp"

template<class type>
inline void Entities::ApplyFunc(void(*func)(type *)) {
	for (auto ent = Ents.begin(); ent != Ents.end(); ++ent) {
		auto e = dynamic_cast<type*>(ent->get());
		if (e == nullptr) continue;
		func(e);
	}
}
