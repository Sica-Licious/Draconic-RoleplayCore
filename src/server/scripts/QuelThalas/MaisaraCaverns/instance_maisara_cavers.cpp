#include "InstanceScript.h"  
#include "ScriptMgr.h"  
#include "maisara_caverns.h"  
  
static constexpr ObjectData creatureData[] =  
{  
    { NPC_MUROJIN, DATA_MUROJIN_AND_NEKRAXX },  
    { NPC_NEKRAXX, DATA_MUROJIN_AND_NEKRAXX }  
};  
  
static constexpr DungeonEncounterData encounters[] =  
{  
    { DATA_MUROJIN_AND_NEKRAXX, {{ 0 /* DungeonEncounter.db2 ID, TBD */ }} }  
};  
  
class instance_maisara_caverns : public InstanceMapScript  
{  
public:  
    instance_maisara_caverns() : InstanceMapScript(MCScriptName, 0 /* mapId, TBD */) { }  
  
    struct instance_maisara_caverns_InstanceMapScript : public InstanceScript  
    {  
        instance_maisara_caverns_InstanceMapScript(InstanceMap* map) : InstanceScript(map)  
        {  
            SetHeaders(DataHeader);  
            SetBossNumber(EncounterCount);  
            LoadObjectData(creatureData, {});  
            LoadDungeonEncounterData(encounters);  
        }  
  
        void OnCreatureCreate(Creature* creature) override  
        {  
            InstanceScript::OnCreatureCreate(creature);  
  
            switch (creature->GetEntry())  
            {  
                case NPC_MUROJIN:  
                    MurojinGUID = creature->GetGUID();  
                    break;  
                case NPC_NEKRAXX:  
                    NekraxxGUID = creature->GetGUID();  
                    break;  
                default:  
                    break;  
            }  
        }  
  
        void OnUnitDeath(Unit* unit) override  
        {  
            InstanceScript::OnUnitDeath(unit);  
  
            if (unit->GetGUID() == NekraxxGUID)  
            {  
                if (Creature* murojin = GetCreature(DATA_MUROJIN_AND_NEKRAXX))  
                    if (murojin->GetGUID() == MurojinGUID)  
                        murojin->AI()->DoAction(ACTION_NEKRAXX_REVIVE_PET);  
            }  
            else if (unit->GetGUID() == MurojinGUID)  
            {  
                if (Creature* nekraxx = instance->GetCreature(NekraxxGUID))  
                    nekraxx->AI()->DoAction(ACTION_MUROJIN_BESTIAL_WRATH);  
            }  
        }  
  
    private:  
        ObjectGuid MurojinGUID;  
        ObjectGuid NekraxxGUID;  
    };  
  
    InstanceScript* GetInstanceScript(InstanceMap* map) const override  
    {  
        return new instance_maisara_caverns_InstanceMapScript(map);  
    }  
};  
  
void AddSC_instance_maisara_caverns()  
{  
    new instance_maisara_caverns();  
}