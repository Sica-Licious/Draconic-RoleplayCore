/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */



#include "Creature.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ScriptedCreature.h"
#include "GameObject.h"
#include "GameObjectAI.h"
#include "PerksProgramMgr.h"
#include "Player.h"

#include "Mail.h"
#include "Item.h"
#include "ObjectMgr.h"
#include "DatabaseEnv.h"
#include <fstream>
#include <sstream>

 // Forward declaration
static void SendStoreMail(Player* player, uint32 itemId, uint32 count);

// --------------------------------------------------
// Proper retail item creation mail function
// --------------------------------------------------
static void SendStoreMail(Player* player, uint32 itemId, uint32 count)
{
    if (!player)
        return;

    CharacterDatabaseTransaction trans = CharacterDatabase.BeginTransaction();

    MailDraft draft("Store Purchase", "Thank you for your purchase!");

    if (Item* item = Item::CreateItem(itemId, count, ItemContext::NONE, player))
    {
        draft.AddItem(item);
        item->SaveToDB(trans);
    }

    // ⭐ CORRECT MailSender for RoleplayCore / older TrinityCore
    MailSender sender((Object*)nullptr, MAIL_STATIONERY_DEFAULT);

    // ⭐ REQUIRED MailReceiver wrapper
    MailReceiver receiver(player);

    draft.SendMailTo(trans, receiver, sender, MAIL_CHECK_MASK_NONE, 0);

    CharacterDatabase.CommitTransaction(trans);
}

// --------------------------------------------------
// Perks Program Vendor Script
// --------------------------------------------------
enum PerksProgramVendorData
{
    GOSSIP_MENU_PERKS_VENDOR_0 = 34391,
    GOSSIP_MENU_PERKS_VENDOR_1 = 30312,
    GOSSIP_MENU_PERKS_VENDOR_2 = 30315,
    GOSSIP_MENU_PERKS_VENDOR_3 = 34385,
};

class npc_perks_program_vendor : public CreatureScript
{
public:
    npc_perks_program_vendor() : CreatureScript("npc_perks_program_vendor") { }

    struct npc_perks_program_vendorAI : public ScriptedAI
    {
        npc_perks_program_vendorAI(Creature* creature) : ScriptedAI(creature) { }

        bool OnGossipSelect(Player* player, uint32 menuId, uint32 gossipListId) override
        {
            switch (menuId)
            {
            case GOSSIP_MENU_PERKS_VENDOR_0:
            case GOSSIP_MENU_PERKS_VENDOR_1:
            case GOSSIP_MENU_PERKS_VENDOR_2:
            case GOSSIP_MENU_PERKS_VENDOR_3:
            {
                if (gossipListId == 0)
                {
                    player->GetSession()->SendPerksProgramVendorList(me->GetGUID());
                    CloseGossipMenuFor(player);
                    return true;
                }

                // Optional test button
                if (gossipListId == 1)
                {
                    SendStoreMail(player, 238994, 1);
                    CloseGossipMenuFor(player);
                    return true;
                }

                return false;
            }

            default:
                return false;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const override
    {
        return new npc_perks_program_vendorAI(creature);
    }
};

// --------------------------------------------------
// Perks Collector Cache Script
// --------------------------------------------------
struct go_perks_collectors_cacheAI : public GameObjectAI
{
    go_perks_collectors_cacheAI(GameObject* go) : GameObjectAI(go) { }

    bool OnGossipHello(Player* player) override
    {
        PerksProgramActivityLists lists = sPerksProgramMgr->GetCurrentMonthActivityLists();
        bool hasPending = false;

        for (int32 id : lists.ThresholdActivityIDs)
        {
            if (player->HasPerksMilestone(id))
            {
                hasPending = true;
                break;
            }
        }

        if (!hasPending)
            return true;

        player->GetSession()->SendPerksProgramCurrencyUpdate();
        return true;
    }
};

// --------------------------------------------------
// Script Registration
// --------------------------------------------------
void AddSC_npc_perks_program_vendor()
{
    new npc_perks_program_vendor();
    RegisterGameObjectAI(go_perks_collectors_cacheAI);
}
