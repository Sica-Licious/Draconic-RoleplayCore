#include "InstanceScript.h"  
#include "ScriptedCreature.h"  
#include "ScriptMgr.h"  
#include "maisara_caverns.h"  
  
enum MurojinSpells  
{  
    SPELL_FREEZING_TRAP    = 1260731,       // still unknown - not provided  
    SPELL_BARRAGE          = 1260643,  
    SPELL_FLANKING_SPEAR   = 1266480,  
    SPELL_OPEN_WOUND       = 1266488, // applied by Flanking Spear  
    SPELL_REVIVE_PET       = 1249789  
};  
  
enum NekraxxSpells  
{  
    SPELL_CARRION_SWOOP    = 1249479,  
    SPELL_FETID_QUILLSTORM = 1243900,  
    SPELL_INFECTED_PINIONS = 1246666,  
    SPELL_BESTIAL_WRATH    = 1249948  
};  
  
enum MurojinEvents  
{  
    EVENT_FREEZING_TRAP  = 1,  
    EVENT_BARRAGE        = 2,  
    EVENT_FLANKING_SPEAR = 3  
};  
  
enum NekraxxEvents  
{  
    EVENT_CARRION_SWOOP    = 1,  
    EVENT_FETID_QUILLSTORM = 2,  
    EVENT_INFECTED_PINIONS = 3  
};  
  
struct boss_murojin : public BossAI  
{  
    boss_murojin(Creature* creature) : BossAI(creature, DATA_MUROJIN_AND_NEKRAXX) { }  
  
    void JustEngagedWith(Unit* who) override  
    {  
        BossAI::JustEngagedWith(who);  
  
        events.ScheduleEvent(EVENT_FREEZING_TRAP, 8s, 12s);  
        events.ScheduleEvent(EVENT_BARRAGE, 5s, 8s);  
        events.ScheduleEvent(EVENT_FLANKING_SPEAR, 15s, 20s);  
    }  
  
    void DoAction(int32 action) override  
    {  
        if (action == ACTION_NEKRAXX_REVIVE_PET)  
            DoCastSelf(SPELL_REVIVE_PET);  
    }  
  
    void UpdateAI(uint32 diff) override  
    {  
        if (!UpdateVictim())  
            return;  
  
        events.Update(diff);  
  
        if (me->HasUnitState(UNIT_STATE_CASTING))  
            return;  
  
        while (uint32 eventId = events.ExecuteEvent())  
        {  
            switch (eventId)  
            {  
                case EVENT_FREEZING_TRAP:  
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))  
                        DoCast(target, SPELL_FREEZING_TRAP);  
                    events.ScheduleEvent(EVENT_FREEZING_TRAP, 15s, 20s);  
                    break;  
                case EVENT_BARRAGE:  
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))  
                        DoCast(target, SPELL_BARRAGE);  
                    events.ScheduleEvent(EVENT_BARRAGE, 18s, 22s);  
                    break;  
                case EVENT_FLANKING_SPEAR:  
                    // Repositions behind current target and applies Open Wound  
                    DoCastVictim(SPELL_FLANKING_SPEAR);  
                    events.ScheduleEvent(EVENT_FLANKING_SPEAR, 20s, 25s);  
                    break;  
                default:  
                    break;  
            }  
  
            if (me->HasUnitState(UNIT_STATE_CASTING))  
                return;  
        }  
  
        DoMeleeAttackIfReady();  
    }  
};  
  
struct boss_nekraxx : public BossAI  
{  
    boss_nekraxx(Creature* creature) : BossAI(creature, DATA_MUROJIN_AND_NEKRAXX) { }  
  
    void JustEngagedWith(Unit* who) override  
    {  
        BossAI::JustEngagedWith(who);  
  
        events.ScheduleEvent(EVENT_CARRION_SWOOP, 10s, 14s);  
        events.ScheduleEvent(EVENT_FETID_QUILLSTORM, 6s, 9s);  
        events.ScheduleEvent(EVENT_INFECTED_PINIONS, 20s);  
    }  
  
    void DoAction(int32 action) override  
    {  
        if (action == ACTION_MUROJIN_BESTIAL_WRATH)  
            DoCastSelf(SPELL_BESTIAL_WRATH, true); // stacking enrage - true = triggered, ignores GCD/interrupt  
    }  
  
    void UpdateAI(uint32 diff) override  
    {  
        if (!UpdateVictim())  
            return;  
  
        events.Update(diff);  
  
        if (me->HasUnitState(UNIT_STATE_CASTING))  
            return;  
  
        while (uint32 eventId = events.ExecuteEvent())  
        {  
            switch (eventId)  
            {  
                case EVENT_CARRION_SWOOP:  
                    if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))  
                        DoCast(target, SPELL_CARRION_SWOOP);  
                    events.ScheduleEvent(EVENT_CARRION_SWOOP, 20s, 25s);  
                    break;  
                case EVENT_FETID_QUILLSTORM:  
                    DoCastAOE(SPELL_FETID_QUILLSTORM);  
                    events.ScheduleEvent(EVENT_FETID_QUILLSTORM, 18s, 22s);  
                    break;  
                case EVENT_INFECTED_PINIONS:  
                    DoCastAOE(SPELL_INFECTED_PINIONS);  
                    events.ScheduleEvent(EVENT_INFECTED_PINIONS, 30s);  
                    break;  
                default:  
                    break;  
            }  
  
            if (me->HasUnitState(UNIT_STATE_CASTING))  
                return;  
        }  
  
        DoMeleeAttackIfReady();  
    }  
};  
  
void AddSC_boss_murojin_and_nekraxx()  
{  
    RegisterCreatureAI(boss_murojin);  
    RegisterCreatureAI(boss_nekraxx);  
}