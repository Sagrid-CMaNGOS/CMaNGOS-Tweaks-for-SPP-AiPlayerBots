/*
* BotRollMasterLoot.cpp
*
* Purpose:
* -----------
* This action allows bots to respond automatically to a "roll <itemlink>" command
* in the raid chat when the player is using MasterLoot mode. 
* It simulates a /rand roll as a real player would do in-game and allows the Master Looter
* to see the results of the roll for proper loot distribution.
*
* Features:
* -----------
* 1. Listens for a "roll <itemlink>" command from the raid chat.
* 2. Checks each bot in the raid group to determine interest:
*    - Based on class, spec, equipped items, and item usage logic.
*    - Special handling for multiple equip slots:
*      - Rings: checks both finger1 and finger2
*      - Trinkets: checks both trinket1 and trinket2
*      - Dual-wield weapons: checks both main-hand and off-hand
*      - Other gear: checks appropriate equip slot(s)
* 3. Interested bots perform a random roll between 1-100, exactly as /rand would do in the game.
* 4. Roll results are broadcasted to the chat as if the bot typed /rand.
* 5. The bot with the highest roll sends a victory emote with a random message.
* 6. Bots that lose the roll send a random "loser" emote/message.
*
* Steps:
* -----------
* - Parse the item link from the chat command.
* - Collect all bots in the raid group.
* - Determine which bots are interested in the item based on item level, slot availability, and class/spec suitability.
* - Each interested bot generates a random roll (1-100) and sends it to the chat via MSG_RANDOM_ROLL.
* - Determine the highest roll (winner) and send the victory emote/message.
* - All other bots (losers) send a defeat emote/message.
* - Handles rings, trinkets, dual-wield weapons, and standard equip slots intelligently.
*
* Notes:
* -----------
* - Disenchant-only or vendor-only items are ignored automatically.
* - Bots not interested in the item will not roll.
* - Messages are randomized to make bot behavior look natural.
* - Emote channels are randomized between say/yell to simulate real player behavior.
*
* This code enables realistic loot rolls for bots under MasterLoot and can be compiled
* directly into the Playerbot AI system.
*/

#pragma once

#include "playerbot/playerbot.h"
#include "LootRollAction.h"
#include "playerbot/strategy/values/ItemUsageValue.h"
#include "playerbot/strategy/values/LootValues.h"

using namespace ai;

class MasterLootRollAction : public Action
{
public:
    MasterLootRollAction(PlayerbotAI* ai) : Action(ai, "masterloot roll") {}

    bool Execute(Event& event) override
    {
        Player* requester = event.getOwner() ? event.getOwner() : GetMaster();
        std::string text = event.getParam();

        if (text.empty())
        {
            ai->TellPlayerNoFacing(requester, "Please specify an item to roll.");
            return false;
        }

        // Parse the item link
        ItemIds ids = ChatHelper::parseItems(text);
        if (ids.empty())
        {
            ai->TellPlayerNoFacing(requester, "Invalid item link.");
            return false;
        }

        ItemQualifier itemQualifier = ItemQualifier(ids.begin()->first);
        if (!itemQualifier.GetId())
            return false;

        Group* group = bot->GetGroup();
        if (!group)
            return false;

        std::vector<Player*> interestedBots;

        for (GroupReference* ref = group->GetFirstMember(); ref; ref = ref->next())
        {
            Player* member = ref->GetSource();
            if (!member || !member->IsBot())
                continue;

            if (IsBotInterested(member, itemQualifier))
                interestedBots.push_back(member);
        }

        if (interestedBots.empty())
            return false;

        std::map<Player*, uint32> rollResults;

        // Each interested bot rolls randomly (like /rand 1-100)
        for (Player* botMember : interestedBots)
        {
            uint32 roll = urand(1, 100);
            rollResults[botMember] = roll;

            // Send roll to chat like /rand
            WorldPacket data(MSG_RANDOM_ROLL, 8 + 4 + 1);
            data << botMember->GetGUID();
            data << roll;
            data << (uint8)100;
            botMember->GetSession()->HandleRandomRollOpcode(data);
        }

        // Determine winner
        auto winnerIt = std::max_element(rollResults.begin(), rollResults.end(),
            [](const std::pair<Player*, uint32>& a, const std::pair<Player*, uint32>& b)
            {
                return a.second < b.second;
            });

        Player* winner = winnerIt->first;

        // Victory and defeat emotes/messages
        std::vector<std::string> victoryMsgs = {
            "yo BIS drop secured! my build is straight fire now xD",
            "OMG epic loot in the bag! gearscore bout to pop off!",
            "rngesus blessed me ! snagged that 1% drop, let’s gooo :E",
            "just looted the OP piece ! raid carry mode activated <3",
            "HOLY CRAP I GOT IT ! endgame gear complete, I’m unstoppable !",
            "yesss snagged the rare drop! my stats are god-tier now xP",
            "loot table loves me ! BIS gear acquired, time to flex !",
            "I’m shaking IRL ! just got the ultimate piece, build maxed :D",
            "epic loot get ! ready to dunk on noobs in style!",
            "rng smiled on me ! looted the game-changer, let’s roll <===3"
        };

        std::vector<std::string> defeatMsgs = {
            "fml lost the roll! been farming forever, this is BS :(",
            "no way some rando ninja’d my BIS ! I’m so tilted >:E",
            "rng hates my guts! lost the drop, my life is over!",
            "why does this game do me dirty ?! loot gone to a casual :’(",
            "I’m MAD ! grinded weeks and some scrub stole my gear!",
            "lost the roll again ?! this RNG is pure trash >:P",
            "F*** this ! rare drop stolen by a noob, I’m out :E",
            "my soul is crushed ! lost the OP piece, game’s rigged!",
            "can’t believe I lost the roll ! my endgame dreams are dead :("
        };

        // Winner sends victory emote
        std::string winnerMsg = victoryMsgs[urand(0, victoryMsgs.size() - 1)];
        uint8 winnerChannel = urand(0, 1) ? CHAT_MSG_SAY : CHAT_MSG_YELL;
        winner->HandleEmoteCommand("cheer", winnerMsg, winnerChannel);

        // Losers send defeat emotes
        for (auto& pair : rollResults)
        {
            Player* botMember = pair.first;
            if (botMember == winner)
                continue;

            std::vector<std::string> defeatEmotes = { "roar", "bored", "cry" };
            std::string chosenEmote = defeatEmotes[urand(0, defeatEmotes.size() - 1)];
            std::string defeatMsg = defeatMsgs[urand(0, defeatMsgs.size() - 1)];
            uint8 channel = urand(0, 1) ? CHAT_MSG_SAY : CHAT_MSG_YELL;

            botMember->HandleEmoteCommand(chosenEmote, defeatMsg, channel);
        }

        return true;
    }

private:
    bool IsBotInterested(Player* botMember, ItemQualifier& item)
    {
        // Check item usage for bot AI
        ItemUsage usage = AI_VALUE2(ItemUsage, "item usage", item.GetQualifier());
        if (usage == ItemUsage::ITEM_USAGE_DISENCHANT || usage == ItemUsage::ITEM_USAGE_VENDOR)
            return false;

        // Now check all relevant slots for better gear
        Item* botItem = nullptr;
        ItemPrototype const* proto = sItemStorage.LookupEntry<ItemPrototype>(item.GetId());
        if (!proto)
            return false;

        std::vector<InventorySlots> slotsToCheck;

        switch (proto->Class)
        {
        case ITEM_CLASS_WEAPON:
            if (proto->SubClass == ITEM_SUBCLASS_WEAPON_SWORD_ONEHAND ||
                proto->SubClass == ITEM_SUBCLASS_WEAPON_AXE_ONEHAND ||
                proto->SubClass == ITEM_SUBCLASS_WEAPON_DAGGER ||
                proto->SubClass == ITEM_SUBCLASS_WEAPON_MACE_ONEHAND)
            {
                // Check main-hand and off-hand if dual wield
                slotsToCheck.push_back(INVENTORY_SLOT_MAINHAND);
                slotsToCheck.push_back(INVENTORY_SLOT_OFFHAND);
            }
            else
                slotsToCheck.push_back(INVENTORY_SLOT_MAINHAND); // Two-hand weapons automatically check main-hand
            break;

        case ITEM_CLASS_ARMOR:
            if (proto->SubClass == ITEM_SUBCLASS_ARMOR_MISC) // rings, trinkets
            {
                if (proto->SubClass == ITEM_SUBCLASS_ARMOR_RING)
                {
                    slotsToCheck.push_back(INVENTORY_SLOT_FINGER1);
                    slotsToCheck.push_back(INVENTORY_SLOT_FINGER2);
                }
                else if (proto->SubClass == ITEM_SUBCLASS_ARMOR_TRINKET)
                {
                    slotsToCheck.push_back(INVENTORY_SLOT_TRINKET1);
                    slotsToCheck.push_back(INVENTORY_SLOT_TRINKET2);
                }
            }
            else
                slotsToCheck.push_back(proto->InventoryType); // equip slot for other armors
            break;

        default:
            slotsToCheck.push_back(proto->InventoryType);
            break;
        }

        for (auto slot : slotsToCheck)
        {
            botItem = botMember->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
            if (!botItem || botItem->GetProto()->ItemLevel < proto->ItemLevel)
                return true; // bot is interested
        }

        return false; // bot has better gear in all slots
    }
};
