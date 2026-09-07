#include "ScriptMgr.h"
#include "Player.h"
#include "Map.h"
#include "Group.h"
#include "LFGMgr.h"
#include "CreatureAI.h"
#include "ScriptedCreature.h"
#include "MotionMaster.h"
#include "EventMap.h"
#include "follower_dungeon.h"

float constexpr FOLLOWER_TANK_SCAN_RANGE = 10.0f;

struct npc_follower_dungeon_base : public ScriptedAI
{
    npc_follower_dungeon_base(Creature* creature) : ScriptedAI(creature) {}

    void FollowOwner()
    {
        if (Unit* owner = me->GetOwner())
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
    }

    void JustReachedHome() override
    {
        FollowOwner();
    }

    void OwnerAttackedBy(Unit* attacker) override
    {
        if (!me->IsEngaged())
            AttackStart(attacker);
    }
};

enum GarrickSpells
{
    SPELL_GARRICK_SHIELD_OF_THE_RIGHTEOUS = 53600,
    SPELL_GARRICK_AVENGERS_SHIELD = 31935,
    SPELL_GARRICK_CONSECRATION = 26573,
    SPELL_GARRICK_BLESSING_OF_FREEDOM = 1044
};

enum GarrickEvents
{
    EVENT_GARRICK_SHIELD_OF_RIGHTEOUS = 1,
    EVENT_GARRICK_AVENGERS_SHIELD,
    EVENT_GARRICK_CONSECRATION
};

struct npc_follower_garrick_tank : public npc_follower_dungeon_base
{
    npc_follower_garrick_tank(Creature* creature) : npc_follower_dungeon_base(creature) {}

    void IsSummonedBy(WorldObject* summoner) override
    {
        if (Unit* owner = summoner->ToUnit())
        {
            me->SetReactState(REACT_DEFENSIVE);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
        }
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_GARRICK_AVENGERS_SHIELD, 2s);
        _events.ScheduleEvent(EVENT_GARRICK_CONSECRATION, 6s);
        _events.ScheduleEvent(EVENT_GARRICK_SHIELD_OF_RIGHTEOUS, 4s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!me->IsEngaged())
        {
            std::list<Creature*> targets;
            me->GetCreatureListWithEntryInGrid(targets, 0, FOLLOWER_TANK_SCAN_RANGE);
            for (Creature* target : targets)
            {
                if (target->IsAlive() && target->IsHostileTo(me) && !target->HasUnitFlag(UNIT_FLAG_NON_ATTACKABLE))
                {
                    me->GetMotionMaster()->Clear();
                    AttackStart(target);
                    break;
                }
            }
        }

        if (!UpdateVictim())
            return;

        _events.Update(diff);
        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_GARRICK_AVENGERS_SHIELD:
                DoCastVictim(SPELL_GARRICK_AVENGERS_SHIELD);
                _events.ScheduleEvent(EVENT_GARRICK_AVENGERS_SHIELD, 15s);
                break;

            case EVENT_GARRICK_CONSECRATION:
                DoCastSelf(SPELL_GARRICK_CONSECRATION);
                _events.ScheduleEvent(EVENT_GARRICK_CONSECRATION, 10s);
                break;

            case EVENT_GARRICK_SHIELD_OF_RIGHTEOUS:
                DoCastVictim(SPELL_GARRICK_SHIELD_OF_THE_RIGHTEOUS);
                _events.ScheduleEvent(EVENT_GARRICK_SHIELD_OF_RIGHTEOUS, 6s);
                break;
            }
        }

        me->DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

enum CrennaSpells
{
    SPELL_CRENNA_REJUVENATION = 774,
    SPELL_CRENNA_HEALING_TOUCH = 5185,
    SPELL_CRENNA_WILD_GROWTH = 48438
};

enum CrennaEvents
{
    EVENT_CRENNA_HEAL_CHECK = 1
};

struct npc_follower_crenna_healer : public npc_follower_dungeon_base
{
    npc_follower_crenna_healer(Creature* creature) : npc_follower_dungeon_base(creature) {}

    void IsSummonedBy(WorldObject* summoner) override
    {
        if (Unit* owner = summoner->ToUnit())
        {
            me->SetReactState(REACT_ASSIST);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
        }

        _events.ScheduleEvent(EVENT_CRENNA_HEAL_CHECK, 1s);
    }

    void UpdateAI(uint32 diff) override
    {
        _events.Update(diff);

        while (uint32 eventId = _events.ExecuteEvent())
        {
            if (eventId == EVENT_CRENNA_HEAL_CHECK)
            {
                Unit* owner = me->GetOwner();
                if (!owner)
                {
                    _events.ScheduleEvent(EVENT_CRENNA_HEAL_CHECK, 1s);
                    break;
                }

                // Scan for lowest HP friendly (player + followers)
                Unit* lowest = owner;
                float lowestPct = lowest->GetHealthPct();

                std::list<Creature*> nearby;
                me->GetCreatureListWithEntryInGrid(nearby, 0, 40.0f);

                for (Creature* ally : nearby)
                {
                    if (!ally->IsAlive())
                        continue;

                    if (!ally->IsFriendlyTo(me))
                        continue;

                    float pct = ally->GetHealthPct();
                    if (pct < lowestPct)
                    {
                        lowest = ally;
                        lowestPct = pct;
                    }
                }

                // Healing logic
                if (lowestPct < 90.0f)
                {
                    if (lowestPct < 40.0f)
                        DoCast(lowest, SPELL_CRENNA_HEALING_TOUCH);
                    else if (!lowest->HasAura(SPELL_CRENNA_REJUVENATION))
                        DoCast(lowest, SPELL_CRENNA_REJUVENATION);

                    // Wild Growth if multiple allies are hurt
                    uint32 hurtCount = 0;
                    for (Creature* ally : nearby)
                        if (ally->IsAlive() && ally->IsFriendlyTo(me) && ally->HealthBelowPct(80))
                            hurtCount++;

                    if (hurtCount >= 3)
                        DoCast(me, SPELL_CRENNA_WILD_GROWTH);
                }

                // ---------------- CLEANSE / DISPEL LOGIC ----------------
                // Druid dispels: Magic (offensive), Curse, Poison
                auto NeedsCleanse = [&](Unit* u)
                    {
                        return u->HasAuraType(SPELL_AURA_MOD_DAMAGE_PERCENT_TAKEN) || // Magic debuff example
                            u->HasAuraType(SPELL_AURA_PERIODIC_DAMAGE) ||          // DoTs
                            u->HasAuraType(SPELL_AURA_MOD_DECREASE_SPEED) ||       // Slows
                            u->HasAuraType(SPELL_AURA_MOD_STUN);                   // Stuns
                    };

                // Scan for any ally needing dispel
                for (Creature* ally : nearby)
                {
                    if (!ally->IsAlive() || !ally->IsFriendlyTo(me))
                        continue;

                    if (NeedsCleanse(ally))
                    {
                        // Nature's Cure (universal druid dispel)
                        DoCast(ally, 88423); // SPELL_NATURES_CURE
                        break;
                    }
                }

                // Player dispel check
                if (NeedsCleanse(owner))
                    DoCast(owner, 88423);

                // ---------------------------------------------------------

                _events.ScheduleEvent(EVENT_CRENNA_HEAL_CHECK, 1s);
            }
        }

        if (!UpdateVictim())
            return;

        me->DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};


enum MeredySpells
{
    SPELL_MEREDY_FIREBALL = 133,
    SPELL_MEREDY_FIRE_BLAST = 108853,
    SPELL_MEREDY_PYROBLAST = 11366
};

enum MeredyEvents
{
    EVENT_MEREDY_FIREBALL = 1,
    EVENT_MEREDY_FIRE_BLAST
};

struct npc_follower_meredy_mage : public npc_follower_dungeon_base
{
    npc_follower_meredy_mage(Creature* creature) : npc_follower_dungeon_base(creature) {}

    void IsSummonedBy(WorldObject* summoner) override
    {
        if (Unit* owner = summoner->ToUnit())
        {
            me->SetReactState(REACT_ASSIST);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());
        }
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_MEREDY_FIREBALL, 1s);
        _events.ScheduleEvent(EVENT_MEREDY_FIRE_BLAST, 8s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        _events.Update(diff);
        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_MEREDY_FIREBALL:
                DoCastVictim(SPELL_MEREDY_FIREBALL);
                _events.ScheduleEvent(EVENT_MEREDY_FIREBALL, 2500ms);
                break;

            case EVENT_MEREDY_FIRE_BLAST:
                DoCastVictim(SPELL_MEREDY_FIRE_BLAST);
                _events.ScheduleEvent(EVENT_MEREDY_FIRE_BLAST, 8s);

                if (Unit* victim = me->GetVictim())
                {
                    if (victim->HealthBelowPct(35) || urand(0, 100) < 20)
                        DoCastVictim(SPELL_MEREDY_PYROBLAST);
                }
                break;
            }
        }

        me->DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

enum AustinSpells
{
    SPELL_AUSTIN_STEADY_SHOT = 56641,
    SPELL_AUSTIN_MULTI_SHOT = 229876,
    SPELL_AUSTIN_KILL_COMMAND = 34026
};

enum AustinPets
{
    NPC_PET_NYX = 209069,
    NPC_PET_ASH = 209068
};

enum AustinEvents
{
    EVENT_AUSTIN_STEADY_SHOT = 1,
    EVENT_AUSTIN_MULTI_SHOT,
    EVENT_AUSTIN_KILL_COMMAND
};

struct npc_follower_austin_hunter : public npc_follower_dungeon_base
{
    npc_follower_austin_hunter(Creature* creature) : npc_follower_dungeon_base(creature) {}

    void IsSummonedBy(WorldObject* summoner) override
    {
        if (Unit* owner = summoner->ToUnit())
        {
            me->SetReactState(REACT_ASSIST);
            me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, me->GetFollowAngle());

            if (Creature* nyx = me->SummonCreature(NPC_PET_NYX, *me, TEMPSUMMON_MANUAL_DESPAWN))
                nyx->GetMotionMaster()->MoveFollow(me, PET_FOLLOW_DIST, me->GetFollowAngle());

            if (Creature* ash = me->SummonCreature(NPC_PET_ASH, *me, TEMPSUMMON_MANUAL_DESPAWN))
                ash->GetMotionMaster()->MoveFollow(me, PET_FOLLOW_DIST, me->GetFollowAngle());
        }
    }

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_AUSTIN_STEADY_SHOT, 2s);
        _events.ScheduleEvent(EVENT_AUSTIN_MULTI_SHOT, 6s);
        _events.ScheduleEvent(EVENT_AUSTIN_KILL_COMMAND, 8s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        _events.Update(diff);
        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_AUSTIN_STEADY_SHOT:
                DoCastVictim(SPELL_AUSTIN_STEADY_SHOT);
                _events.ScheduleEvent(EVENT_AUSTIN_STEADY_SHOT, 2s);
                break;

            case EVENT_AUSTIN_MULTI_SHOT:
                DoCastVictim(SPELL_AUSTIN_MULTI_SHOT);
                _events.ScheduleEvent(EVENT_AUSTIN_MULTI_SHOT, 8s);
                break;

            case EVENT_AUSTIN_KILL_COMMAND:
                DoCastVictim(SPELL_AUSTIN_KILL_COMMAND);
                _events.ScheduleEvent(EVENT_AUSTIN_KILL_COMMAND, 10s);
                break;
            }
        }

        me->DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

struct npc_follower_austin_pet : public ScriptedAI
{
    npc_follower_austin_pet(Creature* creature) : ScriptedAI(creature) {}

    void OwnerAttackedBy(Unit* attacker) override
    {
        if (!me->IsEngaged())
            AttackStart(attacker);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        me->DoMeleeAttackIfReady();
    }
};

enum WrathionSpells
{
    SPELL_WRATHION_STORMSTRIKE = 17364,
    SPELL_WRATHION_LAVA_LASH = 60103,
    SPELL_WRATHION_FLAME_SHOCK = 188389
};

enum WrathionEvents
{
    EVENT_WRATHION_STORMSTRIKE = 1,
    EVENT_WRATHION_LAVA_LASH,
    EVENT_WRATHION_FLAME_SHOCK
};

struct npc_follower_wrathion_shaman : public npc_follower_dungeon_base
{
    npc_follower_wrathion_shaman(Creature* creature) : npc_follower_dungeon_base(creature) {}

    void JustEngagedWith(Unit* /*who*/) override
    {
        _events.ScheduleEvent(EVENT_WRATHION_STORMSTRIKE, 2s);
        _events.ScheduleEvent(EVENT_WRATHION_LAVA_LASH, 5s);
        _events.ScheduleEvent(EVENT_WRATHION_FLAME_SHOCK, 6s);
    }

    void UpdateAI(uint32 diff) override
    {
        if (!UpdateVictim())
            return;

        if (me->HasUnitState(UNIT_STATE_CASTING))
            return;

        _events.Update(diff);
        while (uint32 eventId = _events.ExecuteEvent())
        {
            switch (eventId)
            {
            case EVENT_WRATHION_STORMSTRIKE:
                DoCastVictim(SPELL_WRATHION_STORMSTRIKE);
                _events.ScheduleEvent(EVENT_WRATHION_STORMSTRIKE, 8s);
                break;

            case EVENT_WRATHION_LAVA_LASH:
                DoCastVictim(SPELL_WRATHION_LAVA_LASH);
                _events.ScheduleEvent(EVENT_WRATHION_LAVA_LASH, 8s);
                break;

            case EVENT_WRATHION_FLAME_SHOCK:
                DoCastVictim(SPELL_WRATHION_FLAME_SHOCK);
                _events.ScheduleEvent(EVENT_WRATHION_FLAME_SHOCK, 6s);
                break;
            }
        }

        me->DoMeleeAttackIfReady();
    }

private:
    EventMap _events;
};

class FollowerDungeonPlayerScript : public PlayerScript
{
public:
    FollowerDungeonPlayerScript() : PlayerScript("FollowerDungeonPlayerScript") {}

    void OnMapChanged(Player* player) override
    {
        Map* map = player->GetMap();
        if (!map || !map->IsDungeon())
            return;

        // Do not spawn followers if the player is in a real group
        Group* group = player->GetGroup();
        if (group && group->GetMembersCount() > 1)
            return;

        SpawnFollowers(player);
    }

private:
    void SpawnFollowers(Player* player)
    {
        // Basic example: spawn one of each follower
        // You can replace this with your FollowerPool logic

        // Garrick (Tank)
        player->SummonCreature(161504, *player, TEMPSUMMON_MANUAL_DESPAWN);

        // Crenna (Healer)
        player->SummonCreature(161505, *player, TEMPSUMMON_MANUAL_DESPAWN);

        // Meredy (Mage)
        player->SummonCreature(161506, *player, TEMPSUMMON_MANUAL_DESPAWN);

        // Austin (Hunter)
        player->SummonCreature(209070, *player, TEMPSUMMON_MANUAL_DESPAWN);

        // Wrathion (Shaman)
        player->SummonCreature(209071, *player, TEMPSUMMON_MANUAL_DESPAWN);
    }
};

void AddSC_follower_dungeon_scripts()
{
    new FollowerDungeonPlayerScript();
    RegisterCreatureAI(npc_follower_garrick_tank);
    RegisterCreatureAI(npc_follower_crenna_healer);
    RegisterCreatureAI(npc_follower_meredy_mage);
    RegisterCreatureAI(npc_follower_austin_hunter);
    RegisterCreatureAI(npc_follower_austin_pet);
    RegisterCreatureAI(npc_follower_wrathion_shaman);
}
