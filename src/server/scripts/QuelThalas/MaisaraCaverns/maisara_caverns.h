#ifndef DEF_MAISARA_CAVERNS_H  
#define DEF_MAISARA_CAVERNS_H  

#include "CreatureAIImpl.h"  

#define MCScriptName "instance_maisara_caverns"  
#define DataHeader "MC"  

uint32 constexpr EncounterCount = 1;

enum MCDataTypes
{
    DATA_MUROJIN_AND_NEKRAXX = 0,
    DATA_VORDAZA = 1,
    DATA_RAKTUL = 2
};

enum MCCreatureIds
{
    NPC_MUROJIN = 247570,
    NPC_NEKRAXX = 247572,
    NPC_VORDAZA = 248595, // from wowhead npc id  
    NPC_UNSTABLE_PHANTOM = 250443,
    NPC_RAKTUL = 248605,
    NPC_SOULBIND_TOTEM = 251047, // placeholder, need real entry  
    NPC_LOST_SOUL = 1531,   // placeholder  
    NPC_MALIGNANT_SOUL = 251674
};

enum MCActions
{
    ACTION_NEKRAXX_REVIVE_PET = 1, // Muro'jin revives Nekraxx at 35% hp  
    ACTION_MUROJIN_BESTIAL_WRATH = 2, // Nekraxx enrages when Muro'jin dies  
    ACTION_SPECTRAL_RESIDUE = 3  // granted to all players on Eternal Suffering interrupt  
};

template <class AI, class T>
inline AI* GetMaisaraCavernsAI(T* obj)
{
    return GetInstanceAI<AI>(obj, MCScriptName);
}

#define RegisterMaisaraCavernsCreatureAI(ai_name) RegisterCreatureAIWithFactory(ai_name, GetMaisaraCavernsAI)  

#endif
