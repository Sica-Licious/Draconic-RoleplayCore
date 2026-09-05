#include "InstanceScript.h"  
#include "ScriptedCreature.h"  
#include "ScriptMgr.h"  
#include "maisara_caverns.h"  
  
enum VordazaSpells  
{  
    SPELL_WREST_PHANTOMS        = 1251204,  
    SPELL_NECROTIC_CONVERGENCE  = 1250708,  
    SPELL_DEATHSHROUD           = 1251598,  
    SPELL_DRAIN_SOUL            = 1251554,  
    SPELL_COALESCED_DEATH       = 1252611, // not provided  
    SPELL_UNMAKE                = 1252054, // not provided  
    SPELL_WITHERING_MIASMA      = 1264987  // not provided  
};  
  
enum PhantomSpells  
{  
    SPELL_FINAL_PURSUIT   = 1251775,  
    SPELL_LINGERING_DREAD = 1251813,  
    SPELL_SOULROT         = 1251833 // not provided  
};  
  
enum VordazaEvents  
{  
    EVENT_WREST_PHANTOMS       = 1,  
    EVENT_NECROTIC_CONVERGENCE = 2,  
    EVENT_DRAIN_SOUL           = 3  
};  
  
struct boss_vordaza : public BossAI  
{  
    boss_vordaza(Creature* creature) : BossAI(creature, DATA_VORDAZA) { }  
  
    void JustEngagedWith(Unit* who) override  
    {  
        BossAI::JustEngagedWith(who);  
  
        events.ScheduleEvent(EVENT_WREST_PHANTOMS, 10s, 15s);  
        events.ScheduleEvent(EVENT_NECROTIC_CONVERGENCE, 30s);  
        events.ScheduleEvent(EVENT_DRAIN_SOUL, 8s, 12s);  
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
                case EVENT_WREST_PHANTOMS:  
                    DoCastAOE(SPELL_WREST_PHANTOMS);  
                    events.ScheduleEvent(EVENT_WREST_PHANTOMS, 25s, 30s);  
                    break;  
                case EVENT_NECROTIC_CONVERGENCE:  
                    // Deathshroud makes her immune to interrupts while this resolves  
                    DoCastSelf(SPELL_DEATHSHROUD);  
                    DoCastAOE(SPELL_NECROTIC_CONVERGENCE);  
                    events.ScheduleEvent(EVENT_NECROTIC_CONVERGENCE, 45s, 50s);  
                    break;  
                case EVENT_DRAIN_SOUL:  
                    DoCastVictim(SPELL_DRAIN_SOUL);  
                    events.ScheduleEvent(EVENT_DRAIN_SOUL, 15s, 18s);  
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
  
struct npc_unstable_phantom : public ScriptedAI  
{  
    npc_unstable_phantom(Creature* creature) : ScriptedAI(creature) { }  
  
    void IsSummonedBy(WorldObject* /*summoner*/) override  
    {  
        if (Unit* target = SelectTarget(SelectTargetMethod::Random, 0))  
        {  
            me->GetMotionMaster()->MoveChase(target);  
            DoCastSelf(SPELL_FINAL_PURSUIT);  
        }  
    }  
  
    void JustDied(Unit* /*killer*/) override  
    {  
        DoCastAOE(SPELL_LINGERING_DREAD);  
    }  
};  
  
void AddSC_boss_vordaza()  
{  
    RegisterCreatureAI(boss_vordaza);  
    RegisterCreatureAI(npc_unstable_phantom);  
}