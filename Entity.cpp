#include "Entity.hpp"
#include "Vars.hpp"
#include "Core.hpp"

struct Entities Entities;

Entity* Entities::AddEntity(const ENTITY_TYPE ent, std::vector<Argument>& args) {
	auto ent_ptr = CreateEntity(ent, args);
	if (ent_ptr == nullptr) {
		return nullptr;
	} 
	else {
		auto ptr = ent_ptr.get();
		Ents.push_back(std::move(ent_ptr));
		return ptr;
	}
}

Entity* Entities::AddEntity(const ENTITY_TYPE ent) {
	std::vector<Argument> empty_args;
	return AddEntity(ent, empty_args);
}

void Entities::UpdateEntities() {
	for (auto ent = Ents.begin(); ent != Ents.end(); ++ent) {
		(*ent)->Update();
	}

	CleanEntities();
}

void Entities::RenderEntities() {
	for (auto ent = Ents.begin(); ent != Ents.end(); ++ent) {
		(*ent)->Draw();
	}
}

std::unique_ptr<Entity> Entities::CreateEntity(const ENTITY_TYPE ent, std::vector<Argument>& args) {
	switch (ent) {
	default: return nullptr;

	case ENT_FLUBE:
		return std::move(std::make_unique<Flube>(args));
	case ENT_FOOD:
		return std::move(std::make_unique<Food>(args));
	}
}

void Entities::CleanEntities() {
	Ents.erase(std::remove_if(
		Ents.begin(),
		Ents.end(),
		[](std::unique_ptr<Entity>& E)  {return E->ToDelete(); }
	), Ents.end());
}

Entity::Entity() {
	ID = ID_COUNTER++;
}

void Entity::Collide() {
	on_left_wall = false;
	on_right_wall = false;
	on_ground = false;
	on_ceiling = false;

	auto hitbox = Hitbox();

	if (pos.x < 0) {
		pos.x = 0;
		vel.x = 0;
		on_left_wall = true;
	} else if (pos.x + hitbox.w > 640) {
		pos.x = 640 - hitbox.w;
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

void Entity::UpdatePos() {
	pos = pos + vel * Game.deltaTime;
}

void Entity::Gravity() {
	float gravity = Vars.Get("gravity", Variable(450.0f));
	gravity *= Game.deltaTime;
	vel.y += gravity;
}

void Entity::Goto(Vec2 _pos) {
	auto h = Hitbox();
	pos = (Vec2_F)(_pos - Vec2(h.w, h.h) / 2.0f);
}

void Entity::GotoMouse() {
	Goto(Core.mouse_pos);
}

void Entity::Delete() {
	delete_flag = true;
}

bool Entity::ToDelete() {
	return delete_flag;
}
