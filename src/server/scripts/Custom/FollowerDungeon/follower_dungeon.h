#ifndef DEF_FOLLOWER_DUNGEON_H  
#define DEF_FOLLOWER_DUNGEON_H  

#include "Common.h"  

enum FollowerRole
{
    FOLLOWER_ROLE_TANK = 0,
    FOLLOWER_ROLE_HEALER = 1,
    FOLLOWER_ROLE_DPS = 2
};

struct FollowerTemplate
{
    uint32 AllianceEntry;
    uint32 HordeEntry;
    FollowerRole Role;
};
  
// Placeholder entries - replace with real creature_template IDs  
static constexpr FollowerTemplate FollowerPool[] =
{
    { 9500000, 9500000, FOLLOWER_ROLE_TANK   }, // Garrick - Protection Paladin  
    { 9500001, 9500001, FOLLOWER_ROLE_HEALER }, // Crenna  - Restoration Druid  
    { 9500002, 9500002, FOLLOWER_ROLE_DPS    }, // Meredy  - Fire Mage  
    { 9500003, 9500003, FOLLOWER_ROLE_DPS    }, // Austin  - BM Hunter (summons 2 pets)  
    { 9500004, 9500004, FOLLOWER_ROLE_DPS    }, // Wrathion- Enhancement Shaman  
};
  
// Stored on the summoned creature so its own AI knows what role to play  
enum FollowerData
{
    DATA_FOLLOWER_ROLE = 1
};

#endif
