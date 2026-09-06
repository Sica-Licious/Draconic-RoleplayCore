#ifndef DEF_FOLLOWER_DUNGEON_H  
#define DEF_FOLLOWER_DUNGEON_H  
  
#include "Common.h"  
  
enum FollowerRole  
{  
    FOLLOWER_ROLE_TANK   = 0,  
    FOLLOWER_ROLE_HEALER = 1,  
    FOLLOWER_ROLE_DPS    = 2  
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
    { 90001, 90002, FOLLOWER_ROLE_TANK   },  
    { 90003, 90004, FOLLOWER_ROLE_HEALER },  
    { 90005, 90006, FOLLOWER_ROLE_DPS    },  
    { 90007, 90008, FOLLOWER_ROLE_DPS    },  
    { 90009, 90010, FOLLOWER_ROLE_DPS    },  
};  
  
#endif
