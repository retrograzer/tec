// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#include "lua-system.hpp"

#include <selene.h>

#include "entity.hpp"

namespace tec {
	
	LuaSystem::LuaSystem() {

	}
	
	LuaSystem::~LuaSystem() {

	}
	
	void LuaSystem::Update(const double delta) {
		auto _log = spdlog::get("console_log");
		ProcessCommandQueue();
		
		for (auto itr = ScriptsMap::Begin(); itr != ScriptsMap::End(); itr++) {
			auto entity_id = itr->first;
			if (Entity(entity_id).Has<LuaScript>()) {
				if (LuaScript* lscript = Entity(entity_id).Get<LuaScript>()) {
					if (!lscript->script_name.empty() ) { // Check that have a asigned script with onUpdate function
						//lscript->state["this"].SetObj(Entity(entity_id) //,
							//"add", &Entity::Add,
							//"has", &Entity::Has,
						//	);
							
						lscript->state["onUpdate"](delta); // call to Lua onUpdate method
					}
				}
			}
		}

	}



}
