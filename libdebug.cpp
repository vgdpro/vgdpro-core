/*
 * libdebug.cpp
 *
 *  Created on: 2012-2-8
 *      Author: Argon
 */

#include <cstring>
#include "scriptlib.h"
#include "duel.h"
#include "field.h"
#include "card.h"
#include "effect.h"
#include "ocgapi.h"

int32 scriptlib::debug_message(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	lua_getglobal(L, "tostring");
	lua_pushvalue(L, -2);
	lua_pcall(L, 1, 1, 0);
	interpreter::sprintf(pduel->strbuffer, "%s", lua_tostring(L, -1));
	handle_message(pduel, 2);
	return 0;
}
int32 scriptlib::debug_add_card(lua_State *L) {
	check_param_count(L, 6);
	duel* pduel = interpreter::get_duel_info(L);
	int32 code = (int32)lua_tointeger(L, 1);
	int32 owner = (int32)lua_tointeger(L, 2);
	int32 playerid = (int32)lua_tointeger(L, 3);
	int32 location = (int32)lua_tointeger(L, 4);
	int32 sequence = (int32)lua_tointeger(L, 5);
	int32 position = (int32)lua_tointeger(L, 6);
	int32 proc = lua_toboolean(L, 7);
	if(owner != 0 && owner != 1)
		return 0;
	if(playerid != 0 && playerid != 1)
		return 0;
	if(!pduel->lua->preloaded) {
		pduel->lua->preloaded = TRUE;
		pduel->lua->call_code_function(0, (char*) "PreloadUds", 0, 0);
	}
	if(pduel->game_field->is_location_useable(playerid, location, sequence)) {
		card* pcard = pduel->new_card(code);
		pcard->owner = owner;
		if(location == LOCATION_EXTRA && position == 0)
			position = POS_FACEDOWN_DEFENSE;
		pcard->sendto_param.position = position;
		if(location == LOCATION_PZONE) {
			int32 seq = pduel->game_field->core.duel_rule >= 4 ? sequence * 4 : sequence + 6;
			pduel->game_field->add_card(playerid, pcard, LOCATION_SZONE, seq, TRUE);
		} else {
			pduel->game_field->add_card(playerid, pcard, location, sequence);
		}
		pcard->current.position = position;
		if(!(location & (LOCATION_ONFIELD | LOCATION_PZONE)) || (position & POS_FACEUP)) {
			pcard->enable_field_effect(true);
			pduel->game_field->adjust_instant();
		}
		if(proc)
			pcard->set_status(STATUS_PROC_COMPLETE, TRUE);
		interpreter::card2value(L, pcard);
		return 1;
	} else if(location == LOCATION_MZONE) {
		card* pcard = pduel->new_card(code);
		pcard->owner = owner;
		card* fcard = pduel->game_field->get_field_card(playerid, location, sequence);
		fcard->xyz_add(pcard);
		if(proc)
			pcard->set_status(STATUS_PROC_COMPLETE, TRUE);
		interpreter::card2value(L, pcard);
		return 1;
	}
	return 0;
}
int32 scriptlib::debug_set_player_info(lua_State *L) {
	check_param_count(L, 4);
	duel* pduel = interpreter::get_duel_info(L);
	uint32 playerid = (uint32)lua_tointeger(L, 1);
	uint32 lp = (uint32)lua_tointeger(L, 2);
	uint32 startcount = (uint32)lua_tointeger(L, 3);
	uint32 drawcount = (uint32)lua_tointeger(L, 4);
	if(playerid != 0 && playerid != 1)
		return 0;
	pduel->game_field->player[playerid].lp = lp;
	pduel->game_field->player[playerid].start_count = startcount;
	pduel->game_field->player[playerid].draw_count = drawcount;
	return 0;
}
int32 scriptlib::debug_pre_summon(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 summon_type = (uint32)lua_tointeger(L, 2);
	uint16 summon_location = 0;
	if(lua_gettop(L) > 2)
		summon_location = (uint16)lua_tointeger(L, 3);
	pcard->summon_info = summon_type | (summon_location << 16);
	return 0;
}
int32 scriptlib::debug_pre_equip(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* equip_card = *(card**) lua_touserdata(L, 1);
	card* target = *(card**) lua_touserdata(L, 2);
	if((equip_card->current.location != LOCATION_SZONE)
	        || (target->current.location != LOCATION_MZONE)
	        || (target->current.position & POS_FACEDOWN))
		lua_pushboolean(L, 0);
	else {
		equip_card->equip(target, FALSE);
		equip_card->effect_target_cards.insert(target);
		target->effect_target_owner.insert(equip_card);
		lua_pushboolean(L, 1);
	}
	return 1;
}
int32 scriptlib::debug_pre_set_target(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	check_param(L, PARAM_TYPE_CARD, 2);
	card* t_card = *(card**) lua_touserdata(L, 1);
	card* target = *(card**) lua_touserdata(L, 2);
	t_card->add_card_target(target);
	return 0;
}
int32 scriptlib::debug_pre_add_counter(lua_State *L) {
	check_param_count(L, 2);
	check_param(L, PARAM_TYPE_CARD, 1);
	card* pcard = *(card**) lua_touserdata(L, 1);
	uint32 countertype = (uint32)lua_tointeger(L, 2);
	uint32 count = (uint32)lua_tointeger(L, 3);
	uint16 cttype = countertype;
	auto pr = pcard->counters.emplace(cttype, card::counter_map::mapped_type());
	auto cmit = pr.first;
	if(pr.second) {
		cmit->second[0] = 0;
		cmit->second[1] = 0;
	}
	if(countertype & COUNTER_WITHOUT_PERMIT)
		cmit->second[0] += count;
	else
		cmit->second[1] += count;
	return 0;
}
int32 scriptlib::debug_reload_field_begin(lua_State *L) {
	check_param_count(L, 1);
	duel* pduel = interpreter::get_duel_info(L);
	uint32 flag = (uint32)lua_tointeger(L, 1);
	int32 rule = (int32)lua_tointeger(L, 2);
	pduel->clear();
	pduel->game_field->core.duel_options |= flag;
	if (rule)
		pduel->game_field->core.duel_rule = rule;
	else if (flag & DUEL_OBSOLETE_RULING)
		pduel->game_field->core.duel_rule = 1;
	else
		pduel->game_field->core.duel_rule = CURRENT_RULE;
	return 0;
}
int32 scriptlib::debug_reload_field_end(lua_State *L) {
	duel* pduel = interpreter::get_duel_info(L);
	pduel->game_field->core.shuffle_hand_check[0] = FALSE;
	pduel->game_field->core.shuffle_hand_check[1] = FALSE;
	pduel->game_field->core.shuffle_deck_check[0] = FALSE;
	pduel->game_field->core.shuffle_deck_check[1] = FALSE;
	pduel->game_field->reload_field_info();
	return 0;
}
int32 scriptlib::debug_set_ai_name(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_STRING, 1);
	duel* pduel = interpreter::get_duel_info(L);
	pduel->write_buffer8(MSG_AI_NAME);
	const char* pstr = lua_tostring(L, 1);
	int len = (int)std::strlen(pstr);
	if(len > 100)
		len = 100;
	pduel->write_buffer16(len);
	pduel->write_buffer(pstr, len);
	pduel->write_buffer8(0);
	return 0;
}
int32 scriptlib::debug_show_hint(lua_State *L) {
	check_param_count(L, 1);
	check_param(L, PARAM_TYPE_STRING, 1);
	duel* pduel = interpreter::get_duel_info(L);
	pduel->write_buffer8(MSG_SHOW_HINT);
	const char* pstr = lua_tostring(L, 1);
	int len = (int)std::strlen(pstr);
	if(len > 1024)
		len = 1024;
	pduel->write_buffer16(len);
	pduel->write_buffer(pstr, len);
	pduel->write_buffer8(0);
	return 0;
}

static const struct luaL_Reg debuglib[] = {
	{ "Message", scriptlib::debug_message },
	{ "AddCard", scriptlib::debug_add_card },
	{ "SetPlayerInfo", scriptlib::debug_set_player_info },
	{ "PreSummon", scriptlib::debug_pre_summon },
	{ "PreEquip", scriptlib::debug_pre_equip },
	{ "PreSetTarget", scriptlib::debug_pre_set_target },
	{ "PreAddCounter", scriptlib::debug_pre_add_counter },
	{ "ReloadFieldBegin", scriptlib::debug_reload_field_begin },
	{ "ReloadFieldEnd", scriptlib::debug_reload_field_end },
	{ "SetAIName", scriptlib::debug_set_ai_name },
	{ "ShowHint", scriptlib::debug_show_hint },
	{ NULL, NULL }
};
void scriptlib::open_debuglib(lua_State *L) {
	luaL_newlib(L, debuglib);
	lua_setglobal(L, "Debug");
}
