/*
 * effect.h
 *
 *  Created on: 2010-3-13
 *      Author: Argon
 */

#ifndef EFFECT_H_
#define EFFECT_H_

#include "common.h"
#include "field.h"
#include "effectset.h"
#include <stdlib.h>
#include <vector>
#include <map>

class card;
class duel;
class group;
class effect;
struct tevent;
struct effect_set;
struct effect_set_v;
enum effect_flag : uint32;
enum effect_flag2 : uint32;

class effect {
public:
	int32 ref_handle{ 0 };
	duel* pduel{ nullptr };
	card* owner{ nullptr };
	card* handler{ nullptr };
	uint8 effect_owner{ PLAYER_NONE };
	uint32 description{ 0 };
	uint32 code{ 0 };
	uint32 flag[2]{};
	uint32 id{ 0 };
	uint16 type{ 0 };
	uint16 copy_id{ 0 };
	uint16 range{ 0 };
	uint16 s_range{ 0 };
	uint16 o_range{ 0 };
	uint8 count_limit{ 0 };
	uint8 count_limit_max{ 0 };
	uint16 reset_count{ 0 };
	uint32 reset_flag{ 0 };
	uint32 count_code{ 0 };
	uint32 category{ 0 };
	uint32 hint_timing[2]{};
	uint32 card_type{ 0 };
	uint32 active_type{ 0 };
	uint16 active_location{ 0 };
	uint16 active_sequence{ 0 };
	card* active_handler{ nullptr };
	uint16 status{ 0 };
	std::vector<uint32> label;
	int32 label_object{ 0 };
	int32 condition{ 0 };
	int32 cost{ 0 };
	int32 target{ 0 };
	int32 value{ 0 };
	int32 operation{ 0 };
	uint8 cost_checked{ FALSE };
	uint8 is_granted{ 0 };

	explicit effect(duel* pd);
	~effect() = default;

	int32 is_disable_related();
	int32 is_self_destroy_related();
	int32 is_can_be_forbidden();
	int32 is_available(int32 neglect_disabled = FALSE);
	int32 limit_counter_is_available();
	int32 is_single_ready();
	int32 check_count_limit(uint8 playerid);
	int32 is_activateable(uint8 playerid, const tevent& e, int32 neglect_cond = FALSE, int32 neglect_cost = FALSE, int32 neglect_target = FALSE, int32 neglect_loc = FALSE, int32 neglect_faceup = FALSE);
	int32 is_action_check(uint8 playerid);
	int32 is_activate_ready(effect* reason_effect, uint8 playerid, const tevent& e, int32 neglect_cond = FALSE, int32 neglect_cost = FALSE, int32 neglect_target = FALSE);
	int32 is_activate_ready(uint8 playerid, const tevent& e, int32 neglect_cond = FALSE, int32 neglect_cost = FALSE, int32 neglect_target = FALSE);
	int32 is_condition_check(uint8 playerid, const tevent& e);
	int32 is_activate_check(uint8 playerid, const tevent& e, int32 neglect_cond = FALSE, int32 neglect_cost = FALSE, int32 neglect_target = FALSE);
	int32 is_target(card* pcard);
	int32 is_fit_target_function(card* pcard);
	int32 is_target_player(uint8 playerid);
	int32 is_player_effect_target(card* pcard);
	int32 is_immuned(card* pcard);
	int32 is_chainable(uint8 tp);
	int32 is_hand_trigger();
	int32 reset(uint32 reset_level, uint32 reset_type);
	void dec_count(uint32 playerid = 2);
	void recharge();
	int32 get_value(uint32 extraargs = 0);
	int32 get_value(card* pcard, uint32 extraargs = 0);
	int32 get_value(effect* peffect, uint32 extraargs = 0);
	void get_value(uint32 extraargs, std::vector<int32>* result);
	void get_value(card* pcard, uint32 extraargs, std::vector<int32>* result);
	void get_value(effect* peffect, uint32 extraargs, std::vector<int32>* result);
	int32 check_value_condition(uint32 extraargs = 0);
	void* get_label_object();
	int32 get_speed();
	effect* clone();
	card* get_owner() const;
	uint8 get_owner_player();
	card* get_handler() const;
	uint8 get_handler_player();
	int32 in_range(card* pcard);
	int32 in_range(const chain& ch);
	void set_activate_location();
	void set_active_type();
	uint32 get_active_type();
	int32 get_code_type();

	bool is_flag(effect_flag flag) const {
		return !!(this->flag[0] & flag);
	}
	bool is_flag(effect_flag2 flag) const {
		return !!(this->flag[1] & flag);
	}
};

//status
#define EFFECT_STATUS_AVAILABLE	0x0001
//#define EFFECT_STATUS_ACTIVATED	0x0002
#define EFFECT_STATUS_SPSELF	0x0004

#define EFFECT_COUNT_CODE_OATH	0x10000000
#define EFFECT_COUNT_CODE_DUEL	0x20000000
#define EFFECT_COUNT_CODE_CHAIN	0x40000000
#define EFFECT_COUNT_CODE_SINGLE	0x1

//========== Reset ==========
#define RESET_SELF_TURN		0x10000000
#define RESET_OPPO_TURN		0x20000000
#define RESET_PHASE			0x40000000
#define RESET_CHAIN			0x80000000
#define RESET_EVENT			0x1000
#define RESET_CARD			0x2000
#define RESET_CODE			0x4000
#define RESET_COPY			0x8000

#define RESET_DISABLE		0x00010000
#define RESET_TURN_SET		0x00020000
#define RESET_TOGRAVE		0x00040000
#define RESET_REMOVE		0x00080000
#define RESET_TEMP_REMOVE	0x00100000
#define RESET_TOHAND		0x00200000
#define RESET_TODECK		0x00400000
#define RESET_LEAVE			0x00800000
#define RESET_TOFIELD		0x01000000
#define RESET_CONTROL		0x02000000
#define RESET_OVERLAY		0x04000000
#define RESET_MSCHANGE		0x08000000
#define RESET_TOEXILE		0x20000000

//========== Types ==========
#define EFFECT_TYPE_SINGLE			0x0001	//
#define EFFECT_TYPE_FIELD			0x0002	//
#define EFFECT_TYPE_EQUIP			0x0004	//
#define EFFECT_TYPE_ACTIONS			0x0008	//
#define EFFECT_TYPE_ACTIVATE		0x0010	//
#define EFFECT_TYPE_FLIP			0x0020	//
#define EFFECT_TYPE_IGNITION		0x0040	//
#define EFFECT_TYPE_TRIGGER_O		0x0080	//
#define EFFECT_TYPE_QUICK_O			0x0100	//
#define EFFECT_TYPE_TRIGGER_F		0x0200	//
#define EFFECT_TYPE_QUICK_F			0x0400	//
#define EFFECT_TYPE_CONTINUOUS		0x0800	//
#define EFFECT_TYPE_XMATERIAL		0x1000	//
#define EFFECT_TYPE_GRANT			0x2000	//
#define EFFECT_TYPE_TARGET			0x4000	//

#define EFFECT_TYPES_TRIGGER_LIKE	(EFFECT_TYPE_ACTIVATE | EFFECT_TYPE_TRIGGER_O | EFFECT_TYPE_TRIGGER_F | EFFECT_TYPE_QUICK_O | EFFECT_TYPE_QUICK_F)

//========== Flags ==========
enum effect_flag : uint32 {
	EFFECT_FLAG_INITIAL				= 0x0001,
	EFFECT_FLAG_FUNC_VALUE			= 0x0002,
	EFFECT_FLAG_COUNT_LIMIT			= 0x0004,
	EFFECT_FLAG_FIELD_ONLY			= 0x0008,
	EFFECT_FLAG_CARD_TARGET			= 0x0010,
	EFFECT_FLAG_IGNORE_RANGE		= 0x0020,
	EFFECT_FLAG_ABSOLUTE_TARGET		= 0x0040,
	EFFECT_FLAG_IGNORE_IMMUNE		= 0x0080,
	EFFECT_FLAG_SET_AVAILABLE		= 0x0100,
	EFFECT_FLAG_CANNOT_NEGATE		= 0x0200,
	EFFECT_FLAG_CANNOT_DISABLE		= 0x0400,
	EFFECT_FLAG_PLAYER_TARGET		= 0x0800,
	EFFECT_FLAG_BOTH_SIDE			= 0x1000,
	EFFECT_FLAG_COPY_INHERIT		= 0x2000,
	EFFECT_FLAG_DAMAGE_STEP			= 0x4000,
	EFFECT_FLAG_DAMAGE_CAL			= 0x8000,
	EFFECT_FLAG_DELAY				= 0x10000,
	EFFECT_FLAG_SINGLE_RANGE		= 0x20000,
	EFFECT_FLAG_UNCOPYABLE			= 0x40000,
	EFFECT_FLAG_OATH				= 0x80000,
	EFFECT_FLAG_SPSUM_PARAM			= 0x100000,
//	EFFECT_FLAG_REPEAT				= 0x200000,
	EFFECT_FLAG_NO_TURN_RESET		= 0x400000,
	EFFECT_FLAG_EVENT_PLAYER		= 0x800000,
	EFFECT_FLAG_OWNER_RELATE		= 0x1000000,
	EFFECT_FLAG_CANNOT_INACTIVATE	= 0x2000000,
	EFFECT_FLAG_CLIENT_HINT			= 0x4000000,
	EFFECT_FLAG_CONTINUOUS_TARGET	= 0x8000000,
	EFFECT_FLAG_LIMIT_ZONE			= 0x10000000,
//	EFFECT_FLAG_COF					= 0x20000000,
//	EFFECT_FLAG_CVAL_CHECK			= 0x40000000,
	EFFECT_FLAG_IMMEDIATELY_APPLY	= 0x80000000,
};
enum effect_flag2 : uint32 {
	EFFECT_FLAG2_REPEAT_UPDATE			= 0x0001,
	EFFECT_FLAG2_COF					= 0x0002,
	EFFECT_FLAG2_WICKED					= 0x0004,
	EFFECT_FLAG2_OPTION					= 0x0008,
};
inline effect_flag operator|(effect_flag flag1, effect_flag flag2)
{
	return static_cast<effect_flag>(static_cast<uint32>(flag1) | static_cast<uint32>(flag2));
}
//========== Codes ==========
#define EFFECT_IMMUNE_EFFECT			1	//
#define EFFECT_DISABLE					2	//
#define EFFECT_CANNOT_DISABLE			3	//
#define EFFECT_SET_CONTROL				4	//
#define EFFECT_CANNOT_CHANGE_CONTROL	5	//
#define EFFECT_CANNOT_ACTIVATE			6	//
#define EFFECT_CANNOT_TRIGGER			7	//
#define EFFECT_DISABLE_EFFECT			8	//
#define EFFECT_DISABLE_CHAIN			9	//
#define EFFECT_DISABLE_TRAPMONSTER		10	//
#define EFFECT_CANNOT_INACTIVATE		12	//
#define EFFECT_CANNOT_DISEFFECT			13	//
#define EFFECT_CANNOT_CHANGE_POSITION	14	//
#define EFFECT_TRAP_ACT_IN_HAND			15	//
#define EFFECT_TRAP_ACT_IN_SET_TURN		16	//
#define EFFECT_REMAIN_FIELD				17	//
#define EFFECT_MONSTER_SSET				18	//
#define EFFECT_CANNOT_SUMMON			20	//
#define EFFECT_CANNOT_FLIP_SUMMON		21	//
#define EFFECT_CANNOT_SPECIAL_SUMMON	22	//
#define EFFECT_CANNOT_MSET				23	//
#define EFFECT_CANNOT_SSET				24	//
#define EFFECT_CANNOT_DRAW				25	//
#define EFFECT_CANNOT_DISABLE_SUMMON	26	//
#define EFFECT_CANNOT_DISABLE_SPSUMMON	27	//
#define EFFECT_SET_SUMMON_COUNT_LIMIT	28	//
#define EFFECT_EXTRA_SUMMON_COUNT		29	//
#define EFFECT_SPSUMMON_CONDITION		30	//
#define EFFECT_REVIVE_LIMIT				31	//
#define EFFECT_SUMMON_PROC				32	//
#define EFFECT_LIMIT_SUMMON_PROC		33	//
#define EFFECT_SPSUMMON_PROC			34	//
#define EFFECT_CANMOVE_PARALLEL		    97	//
#define EFFECT_EXTRA_SET_COUNT			35	//
#define EFFECT_SET_PROC					36	//
#define EFFECT_LIMIT_SET_PROC			37	//
#define EFFECT_DIVINE_LIGHT				38	//
#define EFFECT_CANNOT_DISABLE_FLIP_SUMMON	39	//
#define EFFECT_INDESTRUCTABLE			40	//
#define EFFECT_INDESTRUCTABLE_EFFECT	41	//
#define EFFECT_INDESTRUCTABLE_BATTLE	42	//
#define EFFECT_UNRELEASABLE_SUM			43	//
#define EFFECT_UNRELEASABLE_NONSUM		44	//
#define EFFECT_DESTROY_SUBSTITUTE		45	//
#define EFFECT_CANNOT_RELEASE			46	//
#define EFFECT_INDESTRUCTABLE_COUNT		47	//
#define EFFECT_UNRELEASABLE_EFFECT		48	//
#define EFFECT_DESTROY_REPLACE			50	//
#define EFFECT_RELEASE_REPLACE			51	//
#define EFFECT_SEND_REPLACE				52	//
#define EFFECT_CANNOT_DISCARD_HAND		55	//
#define EFFECT_CANNOT_DISCARD_DECK		56	//
#define EFFECT_CANNOT_USE_AS_COST		57	//
#define EFFECT_CANNOT_PLACE_COUNTER		58	//
#define EFFECT_CANNOT_TO_GRAVE_AS_COST	59	//
#define EFFECT_LEAVE_FIELD_REDIRECT		60	//
#define EFFECT_TO_HAND_REDIRECT			61	//
#define EFFECT_TO_DECK_REDIRECT			62	//
#define EFFECT_TO_GRAVE_REDIRECT		63	//
#define EFFECT_REMOVE_REDIRECT			64	//
#define EFFECT_CANNOT_TO_HAND			65	//
#define EFFECT_CANNOT_TO_DECK			66	//
#define EFFECT_CANNOT_REMOVE			67	//
#define EFFECT_CANNOT_TO_GRAVE			68	//
#define EFFECT_CANNOT_TO_EXILE          372
#define EFFECT_CANNOT_TURN_SET			69	//
#define EFFECT_CANNOT_BE_BATTLE_TARGET	70	//
#define EFFECT_CANNOT_BE_EFFECT_TARGET	71	//
#define EFFECT_IGNORE_BATTLE_TARGET		72	//
#define EFFECT_CANNOT_DIRECT_ATTACK		73	//
#define EFFECT_DIRECT_ATTACK			74	//
#define EFFECT_DUAL_STATUS				75	//
#define EFFECT_EQUIP_LIMIT				76	//
#define EFFECT_DUAL_SUMMONABLE			77	//
#define EFFECT_UNION_LIMIT				78	//
#define EFFECT_REVERSE_DAMAGE			80	//
#define EFFECT_REVERSE_RECOVER			81	//
#define EFFECT_CHANGE_DAMAGE			82	//
#define EFFECT_REFLECT_DAMAGE			83	//
#define EFFECT_CANNOT_ATTACK			85	//
#define EFFECT_CANNOT_ATTACK_ANNOUNCE	86	//
#define EFFECT_CANNOT_CHANGE_POS_E		87	//
#define EFFECT_ACTIVATE_COST			90	//
#define EFFECT_SUMMON_COST				91	//
#define EFFECT_SPSUMMON_COST			92	//
#define EFFECT_FLIPSUMMON_COST			93	//
#define EFFECT_MSET_COST				94	//
#define EFFECT_SSET_COST				95	//
#define EFFECT_ATTACK_COST				96	//

#define EFFECT_UPDATE_ATTACK			100	//
#define EFFECT_SET_ATTACK				101	//
#define EFFECT_SET_ATTACK_FINAL			102	//
#define EFFECT_SET_BASE_ATTACK			103	//
#define EFFECT_UPDATE_DEFENSE			104	//
#define EFFECT_SET_DEFENSE				105	//
#define EFFECT_SET_DEFENSE_FINAL		106	//
#define EFFECT_SET_BASE_DEFENSE			107	//
#define EFFECT_REVERSE_UPDATE			108	//
#define EFFECT_SWAP_AD					109	//
#define EFFECT_SWAP_BASE_AD				110	//
#define EFFECT_SET_BASE_ATTACK_FINAL	111	//
#define EFFECT_SET_BASE_DEFENSE_FINAL	112	//
#define EFFECT_ADD_CODE					113	//
#define EFFECT_CHANGE_CODE				114	//
#define EFFECT_ADD_TYPE					115	//
#define EFFECT_REMOVE_TYPE				116	//
#define EFFECT_CHANGE_TYPE				117	//
#define EFFECT_ADD_RACE					120	//
#define EFFECT_REMOVE_RACE				121	//
#define EFFECT_CHANGE_RACE				122	//
#define EFFECT_ADD_ATTRIBUTE			125	//
#define EFFECT_REMOVE_ATTRIBUTE			126	//
#define EFFECT_CHANGE_ATTRIBUTE			127	//
#define EFFECT_CHANGE_COUNTRY		128	//
#define EFFECT_UPDATE_LEVEL				130	//
#define EFFECT_CHANGE_LEVEL				131	//
#define EFFECT_UPDATE_RANK				132 //
#define EFFECT_CHANGE_RANK				133 //
#define EFFECT_UPDATE_LSCALE			134 //
#define EFFECT_CHANGE_LSCALE			135 //
#define EFFECT_UPDATE_RSCALE			136 //
#define EFFECT_CHANGE_RSCALE			137 //
#define EFFECT_SET_POSITION				140 //
#define EFFECT_SELF_DESTROY				141 //
#define EFFECT_SELF_TOGRAVE				142
#define EFFECT_DOUBLE_TRIBUTE			150
#define EFFECT_DECREASE_TRIBUTE			151
#define EFFECT_DECREASE_TRIBUTE_SET		152
#define EFFECT_EXTRA_RELEASE			153
#define EFFECT_TRIBUTE_LIMIT			154
#define EFFECT_EXTRA_RELEASE_SUM		155
//#define EFFECT_TRIPLE_TRIBUTE			156
#define EFFECT_ADD_EXTRA_TRIBUTE		157
#define EFFECT_EXTRA_RELEASE_NONSUM		158
#define EFFECT_PUBLIC					160
#define EFFECT_COUNTER_PERMIT			0x10000
#define EFFECT_COUNTER_LIMIT			0x20000
#define EFFECT_RCOUNTER_REPLACE			0x30000
#define EFFECT_LPCOST_CHANGE			170
#define EFFECT_LPCOST_REPLACE			171
#define EFFECT_SKIP_DP					180
#define EFFECT_SKIP_SP					181
#define EFFECT_SKIP_M1					182
#define EFFECT_SKIP_BP					183
#define EFFECT_SKIP_M2					184
#define EFFECT_CANNOT_BP				185
#define EFFECT_CANNOT_M2				186
#define EFFECT_CANNOT_EP				187
#define EFFECT_SKIP_TURN				188
#define EFFECT_SKIP_EP					189
#define EFFECT_DEFENSE_ATTACK			190
#define EFFECT_MUST_ATTACK				191
#define EFFECT_FIRST_ATTACK				192
#define EFFECT_ATTACK_ALL				193
#define EFFECT_EXTRA_ATTACK				194
//#define EFFECT_MUST_BE_ATTACKED			195
#define EFFECT_ONLY_BE_ATTACKED			196
#define EFFECT_ATTACK_DISABLED			197
#define EFFECT_NO_BATTLE_DAMAGE			200
#define EFFECT_AVOID_BATTLE_DAMAGE		201
#define EFFECT_REFLECT_BATTLE_DAMAGE	202
#define EFFECT_PIERCE					203
#define EFFECT_BATTLE_DESTROY_REDIRECT	204
#define EFFECT_BATTLE_DAMAGE_TO_EFFECT	205
#define EFFECT_BOTH_BATTLE_DAMAGE		206
#define EFFECT_ALSO_BATTLE_DAMAGE		207
#define EFFECT_CHANGE_BATTLE_DAMAGE		208
#define EFFECT_TOSS_COIN_REPLACE		220
#define EFFECT_TOSS_DICE_REPLACE		221
#define EFFECT_FUSION_MATERIAL			230
#define EFFECT_CHAIN_MATERIAL			231
#define EFFECT_EXTRA_SYNCHRO_MATERIAL	232
#define EFFECT_XYZ_MATERIAL				233
#define EFFECT_FUSION_SUBSTITUTE		234
#define EFFECT_CANNOT_BE_FUSION_MATERIAL	235
#define EFFECT_CANNOT_BE_SYNCHRO_MATERIAL	236
#define EFFECT_SYNCHRO_MATERIAL_CUSTOM		237
#define EFFECT_CANNOT_BE_XYZ_MATERIAL		238
#define EFFECT_CANNOT_BE_LINK_MATERIAL		239
#define EFFECT_SYNCHRO_LEVEL				240
#define EFFECT_RITUAL_LEVEL					241
#define EFFECT_XYZ_LEVEL					242
#define EFFECT_EXTRA_RITUAL_MATERIAL		243
#define EFFECT_NONTUNER						244
#define EFFECT_OVERLAY_REMOVE_REPLACE		245
#define EFFECT_SCRAP_CHIMERA				246
#define EFFECT_TUNE_MAGICIAN_X				247
#define EFFECT_PRE_MONSTER				250
#define EFFECT_MATERIAL_CHECK			251
#define EFFECT_DISABLE_FIELD			260
#define EFFECT_USE_EXTRA_MZONE			261
#define EFFECT_USE_EXTRA_SZONE			262
#define EFFECT_MAX_MZONE				263
#define EFFECT_MAX_SZONE				264
#define EFFECT_MUST_USE_MZONE			265
#define EFFECT_HAND_LIMIT				270
#define EFFECT_DRAW_COUNT				271
#define EFFECT_SPIRIT_DONOT_RETURN		280
#define EFFECT_SPIRIT_MAYNOT_RETURN		281
#define EFFECT_CHANGE_ENVIRONMENT		290
#define EFFECT_NECRO_VALLEY				291
#define EFFECT_FORBIDDEN				292
#define EFFECT_NECRO_VALLEY_IM			293
#define EFFECT_REVERSE_DECK				294
#define EFFECT_REMOVE_BRAINWASHING		295
#define EFFECT_BP_TWICE					296
#define EFFECT_UNIQUE_CHECK				297
#define EFFECT_MATCH_KILL				300
#define EFFECT_SYNCHRO_CHECK			310
#define EFFECT_QP_ACT_IN_NTPHAND		311
#define EFFECT_MUST_BE_SMATERIAL		312
#define EFFECT_TO_GRAVE_REDIRECT_CB		313
#define EFFECT_CHANGE_INVOLVING_BATTLE_DAMAGE	314
//#define EFFECT_CHANGE_RANK_FINAL		315
#define EFFECT_MUST_BE_FMATERIAL		316
#define EFFECT_MUST_BE_XMATERIAL		317
#define EFFECT_MUST_BE_LMATERIAL		318
#define EFFECT_SPSUMMON_PROC_G			320
#define EFFECT_SPSUMMON_COUNT_LIMIT		330
#define EFFECT_LEFT_SPSUMMON_COUNT		331
#define EFFECT_CANNOT_SELECT_BATTLE_TARGET	332
#define EFFECT_CANNOT_SELECT_EFFECT_TARGET	333
#define EFFECT_ADD_SETCODE				334
#define EFFECT_NO_EFFECT_DAMAGE			335
//#define EFFECT_UNSUMMONABLE_CARD		336
#define EFFECT_DISCARD_COST_CHANGE		338
#define EFFECT_HAND_SYNCHRO				339
#define EFFECT_ADD_FUSION_CODE			340
#define EFFECT_ADD_FUSION_SETCODE		341
#define EFFECT_ONLY_ATTACK_MONSTER		343
#define EFFECT_MUST_ATTACK_MONSTER		344
#define EFFECT_PATRICIAN_OF_DARKNESS	345
#define EFFECT_EXTRA_ATTACK_MONSTER		346
#define EFFECT_UNION_STATUS				347
#define EFFECT_OLDUNION_STATUS			348
//#define EFFECT_ADD_FUSION_ATTRIBUTE		349
//#define EFFECT_REMOVE_FUSION_ATTRIBUTE	350
#define EFFECT_CHANGE_FUSION_ATTRIBUTE	351
#define EFFECT_EXTRA_FUSION_MATERIAL	352
#define EFFECT_TUNER_MATERIAL_LIMIT		353
#define EFFECT_ADD_LINK_CODE				354
//#define EFFECT_ADD_LINK_SETCODE			355
#define EFFECT_ADD_LINK_ATTRIBUTE		356
#define EFFECT_ADD_LINK_RACE			357
#define EFFECT_EXTRA_LINK_MATERIAL		358
#define EFFECT_QP_ACT_IN_SET_TURN		359
#define EFFECT_EXTRA_PENDULUM_SUMMON	360
#define EFFECT_MATERIAL_LIMIT			361
#define EFFECT_SET_BATTLE_ATTACK		362
#define EFFECT_SET_BATTLE_DEFENSE		363
#define EFFECT_OVERLAY_RITUAL_MATERIAL	364
#define EFFECT_CHANGE_GRAVE_ATTRIBUTE	365
#define EFFECT_CHANGE_GRAVE_RACE		366
#define EFFECT_ACTIVATION_COUNT_LIMIT	367
#define EFFECT_LIMIT_SPECIAL_SUMMON_POSITION	368
#define EFFECT_TUNER					369
#define EFFECT_KAISER_COLOSSEUM			370
#define EFFECT_REPLACE_DAMAGE			371

//#define EVENT_STARTUP		1000
#define EVENT_FLIP			1001
#define EVENT_FREE_CHAIN	1002
#define EVENT_DESTROY		1010
#define EVENT_REMOVE		1011
#define EVENT_EXILE		    1811
#define EVENT_TO_HAND		1012
#define EVENT_TO_DECK		1013
#define EVENT_TO_GRAVE		1014
#define EVENT_LEAVE_FIELD	1015
#define EVENT_CHANGE_POS	1016
#define EVENT_RELEASE		1017
#define EVENT_DISCARD		1018
#define EVENT_LEAVE_FIELD_P	1019
#define EVENT_CHAIN_SOLVING		1020
#define EVENT_CHAIN_ACTIVATING	1021
#define EVENT_CHAIN_SOLVED		1022
//#define EVENT_CHAIN_ACTIVATED	1023
#define EVENT_CHAIN_NEGATED		1024
#define EVENT_CHAIN_DISABLED	1025
#define EVENT_CHAIN_END			1026
#define EVENT_CHAINING			1027
#define EVENT_BECOME_TARGET		1028
#define EVENT_DESTROYED			1029
#define EVENT_MOVE				1030
#define EVENT_LEAVE_GRAVE		1031
#define EVENT_LEAVE_DECK		1032
#define EVENT_ADJUST			1040
#define EVENT_BREAK_EFFECT		1050
#define EVENT_SUMMON_SUCCESS		1100
#define EVENT_FLIP_SUMMON_SUCCESS	1101
#define EVENT_SPSUMMON_SUCCESS		1102
#define EVENT_SUMMON				1103
#define EVENT_FLIP_SUMMON			1104
#define EVENT_SPSUMMON				1105
#define EVENT_MSET					1106
#define EVENT_SSET					1107
#define EVENT_BE_MATERIAL			1108
#define EVENT_BE_PRE_MATERIAL			1109
#define EVENT_DRAW					1110
#define EVENT_DAMAGE				1111
#define EVENT_RECOVER				1112
#define EVENT_PREDRAW				1113
#define EVENT_SUMMON_NEGATED		1114
#define EVENT_FLIP_SUMMON_NEGATED	1115
#define EVENT_SPSUMMON_NEGATED		1116
#define EVENT_SPSUMMON_SUCCESS_G_P	1117
#define EVENT_CONTROL_CHANGED		1120
#define EVENT_EQUIP					1121
#define EVENT_ATTACK_ANNOUNCE		1130
#define EVENT_BE_BATTLE_TARGET		1131
#define EVENT_BATTLE_START			1132
#define EVENT_BATTLE_CONFIRM		1133
#define EVENT_PRE_DAMAGE_CALCULATE	1134
//#define EVENT_DAMAGE_CALCULATING	1135
#define EVENT_PRE_BATTLE_DAMAGE		1136
//#define EVENT_BATTLE_END			1137
#define EVENT_BATTLED				1138
#define EVENT_BATTLE_DESTROYING		1139
#define EVENT_BATTLE_DESTROYED		1140
#define EVENT_DAMAGE_STEP_END		1141
#define EVENT_ATTACK_DISABLED		1142
#define EVENT_BATTLE_DAMAGE			1143
#define EVENT_TOSS_DICE				1150
#define EVENT_TOSS_COIN				1151
#define EVENT_TOSS_COIN_NEGATE		1152
#define EVENT_TOSS_DICE_NEGATE		1153
#define EVENT_LEVEL_UP				1200
#define EVENT_PAY_LPCOST			1201
#define EVENT_DETACH_MATERIAL		1202
#define EVENT_TURN_END				1210
#define EVENT_PHASE					0x1000
#define EVENT_PHASE_START			0x2000
#define EVENT_ADD_COUNTER			0x10000
#define EVENT_REMOVE_COUNTER		0x20000
#define EVENT_CUSTOM				0x10000000

constexpr int32 DOUBLE_DAMAGE = 0x80000000;
constexpr int32 HALF_DAMAGE = 0x80000001;

// flag effect
#define EFFECT_FLAG_EFFECT	0x20000000
#define MAX_CARD_ID			0xfffffff

// The type of effect code
#define CODE_CUSTOM		1	// header + id (28 bits)
#define CODE_COUNTER	2	// header + counter_id (16 bits)
#define CODE_PHASE		3	// header + phase_id (12 bits)
#define CODE_VALUE		4	// numeric value, max = 4095

const std::unordered_set<uint32> continuous_event{
	EVENT_ADJUST,
	EVENT_BREAK_EFFECT,
	EVENT_TURN_END,
	EVENT_PRE_BATTLE_DAMAGE,
	EVENT_SPSUMMON_SUCCESS_G_P,
};
bool is_continuous_event(uint32 code);

#endif /* EFFECT_H_ */
