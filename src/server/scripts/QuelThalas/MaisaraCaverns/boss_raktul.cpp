#include "InstanceScript.h"  
#include "ScriptedCreature.h"  
#include "ScriptMgr.h"  
#include "maisara_caverns.h"  
  
enum RaktulSpells  
{  
    SPELL_SPIRITBREAKER      = 1251023,  
    SPELL_CRUSH_SOULS        = 1252676,  
    SPELL_SOULBIND           = 1252777,  
    SPELL_SOULRENDING_ROAR   = 1253788,  
    SPELL_WITHERING_SOUL     = 1253844,  
    SPELL_ETERNAL_SUFFERING  = 1254010,  
    SPELL_SPECTRAL_RESIDUE   = 1255629,  
    SPELL_DEATHGORGED_VESSEL = 1252864,  
    SPELL_SOUL_EXPULSION     = 1253909,  
    SPELL_SHATTERED_TOTEM    = 1259810,  
    SPELL_CHILL_OF_DEATH     = 1252816, // not provided  
    SPELL_VOLATILE_ESSENCE   = 1248980, // not provided  
    SPELL_SPECTRAL_DECAY     = 1266723, // not provided  
    SPELL_OPEN_WOUND_TOTEM   = 1254175, // Cries of the Fallen, not provided  
    SPELL_CRIES_OF_THE_FALLEN = 1254175 // not provided  
};  
  
enum RaktulEvents  
{  
    EVENT_CRUSH_SOULS        = 1,  
    EVENT_SPIRITBREAKER      = 2,  
    EVENT_DEATHGORGED_VESSEL = 3,  
    EVENT_SOULRENDING_ROAR   = 4  
};  
  
struct boss_raktul : public BossAI  
{  
    boss_raktul(Creature* creature) : BossAI(creature, DATA_RAKTUL) { }  
  
    void JustEngagedWith(Unit* who) override  
    {  
        BossAI::JustEngagedWith(who);  
  
        events.ScheduleEvent(EVENT_CRUSH_SOULS, 10s, 14s);  
        events.ScheduleEvent(EVENT_SPIRITBREAKER, 16s, 20s);  
        events.ScheduleEvent(EVENT_DEATHGORGED_VESSEL, 6s, 6s);  
        events.ScheduleEvent(EVENT_SOULRENDING_ROAR, 40s);  
    }  
  
    // Granted to raid when Eternal Suffering (cast by Malignant Soul) is interrupted  
    void DoAction(int32 action) override  
    {  
        if (action == ACTION_SPECTRAL_RESIDUE)  
            DoCastAOE(SPELL_SPECTRAL_RESIDUE);  
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
                case EVENT_CRUSH_SOULS:  
                    // Repeated leap-slams; each impact plants a Soulbind Totem  
                    DoCastSelf(SPELL_CRUSH_SOULS);  
                    events.ScheduleEvent(EVENT_CRUSH_SOULS, 22s, 26s);  
                    break;  
                case EVENT_SPIRITBREAKER:  
                    DoCastVictim(SPELL_SPIRITBREAKER);  
                    events.ScheduleEvent(EVENT_SPIRITBREAKER, 20s, 25s);  
                    break;  
                case EVENT_DEATHGORGED_VESSEL:  
                    DoCastAOE(SPELL_DEATHGORGED_VESSEL);  
                    events.ScheduleEvent(EVENT_DEATHGORGED_VESSEL, 6s);  
                    break;  
                case EVENT_SOULRENDING_ROAR:  
                    // Applies Withering Soul, stuns self into Soul Expulsion trance  
                    DoCastAOE(SPELL_SOULRENDING_ROAR);  
                    DoCastSelf(SPELL_SOUL_EXPULSION, true);  
                    DoCastSelf(SPELL_SHATTERED_TOTEM, true); // shatters remaining totems  
                    events.ScheduleEvent(EVENT_SOULRENDING_ROAR, 90s);  
                    break;  
                default:  
                    break;  
            }  
  
            if (me->HasUnitState(UNIT_STATE_CASTING))  
                return;  
        }  
  
        me->DoMeleeAttackIfReady();
    }  
};  
  
// Planted by Crush Souls impacts  
struct npc_soulbind_totem : public ScriptedAI  
{  
    npc_soulbind_totem(Creature* creature) : ScriptedAI(creature) { }  
  
    void JustAppeared() override  
    {  
        DoCastVictim(SPELL_SOULBIND);  
    }  
};  
  
// Restless Masses adds summoned during Soulrending Roar / Soul Expulsion  
struct npc_lost_soul : public ScriptedAI  
{  
    npc_lost_soul(Creature* creature) : ScriptedAI(creature) { }  
  
    void MoveInLineOfSight(Unit* who) override  
    {  
        ScriptedAI::MoveInLineOfSight(who);  
  
        if (who->IsPlayer() && me->IsWithinMeleeRange(who))  
            DoCastSelf(SPELL_CRIES_OF_THE_FALLEN);  
    }  
};  
  
struct npc_malignant_soul : public ScriptedAI  
{  
    npc_malignant_soul(Creature* creature) : ScriptedAI(creature) { }  
  
    void JustAppeared() override  
    {  
        DoCastSelf(SPELL_ETERNAL_SUFFERING);  
    }  
  
    // On interrupt, dissipates and grants Spectral Residue to all players  
    void SpellInterrupted(SpellInfo const* /*spellInfo*/) // no override — not a real CreatureAI virtual, dead code  
    {
        if (Creature* raktul = me->GetInstanceScript() ? me->GetInstanceScript()->GetCreature(DATA_RAKTUL) : nullptr)
            raktul->AI()->DoAction(ACTION_SPECTRAL_RESIDUE);

        me->DespawnOrUnsummon();
    }
};  
  
void AddSC_boss_raktul()  
{  
    RegisterCreatureAI(boss_raktul);  
    RegisterCreatureAI(npc_soulbind_totem);  
    RegisterCreatureAI(npc_lost_soul);  
    RegisterCreatureAI(npc_malignant_soul);  
}
