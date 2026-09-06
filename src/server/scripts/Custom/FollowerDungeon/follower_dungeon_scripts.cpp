#include "ScriptMgr.h"  
#include "Player.h"  
#include "Map.h"  
#include "Group.h"  
#include "LFGMgr.h"  
#include "follower_dungeon.h"  

class FollowerDungeonPlayerScript : public PlayerScript
{
public:
    FollowerDungeonPlayerScript() : PlayerScript("FollowerDungeonPlayerScript") {}

    void OnMapChanged(Player* player) override
    {
        Map* map = player->GetMap();
        if (!map->IsDungeon())
            return;

        // Only spawn followers if player entered alone (no real group members)  
        Group* group = player->GetGroup();
        if (group && group->GetMembersCount() > 1)
            return;

        uint8 role = sLFGMgr->GetRoles(player->GetGUID()) & ~lfg::PLAYER_ROLE_LEADER;
        SpawnFollowers(player, role);
    }

private:
    void SpawnFollowers(Player* player, uint8 role)
    {
        bool isAlliance = (player->GetTeam() == ALLIANCE);

        uint8 neededTank = 1, neededHealer = 1, neededDps = 3;
        switch (role)
        {
        case lfg::PLAYER_ROLE_TANK:
            neededTank = 0; neededHealer = 1; neededDps = 3;
            break;
        case lfg::PLAYER_ROLE_HEALER:
            neededTank = 1; neededHealer = 0; neededDps = 3;
            break;
        case lfg::PLAYER_ROLE_DAMAGE:
        default:
            neededTank = 1; neededHealer = 1; neededDps = 2;
            break;
        }

        for (FollowerTemplate const& tmpl : FollowerPool)
        {
            uint8* counter = nullptr;
            switch (tmpl.Role)
            {
            case FOLLOWER_ROLE_TANK:   counter = &neededTank;   break;
            case FOLLOWER_ROLE_HEALER: counter = &neededHealer; break;
            case FOLLOWER_ROLE_DPS:    counter = &neededDps;    break;
            }

            if (!counter || *counter == 0)
                continue;

            uint32 entry = isAlliance ? tmpl.AllianceEntry : tmpl.HordeEntry;
            player->SummonCreature(entry, *player, TEMPSUMMON_MANUAL_DESPAWN);
            --(*counter);
        }
    }
};

void AddSC_follower_dungeon_scripts()
{
    new FollowerDungeonPlayerScript();
}
