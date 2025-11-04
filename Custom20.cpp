// ==================================================================
// Add in PlayerbotFactory.cpp - Custom Level 20 Mount Initialization
// ==================================================================

#ifdef MANGOSBOT_ZERO

std::vector<uint32> custom20;
switch (bot->getRace())
{
    case RACE_HUMAN:      custom20 = {34000, 34001}; break; // Honor Steed, Harvest Steed
    case RACE_ORC:        custom20 = {50003, 50004}; break;
    case RACE_DWARF:      custom20 = {50005, 50006}; break;
    case RACE_NIGHTELF:   custom20 = {50007, 50008}; break;  
    case RACE_UNDEAD:     custom20 = {50009, 50010}; break;
    case RACE_TAUREN:     custom20 = {50011, 50012}; break;
    case RACE_GNOME:      custom20 = {50013, 50014}; break;
    case RACE_TROLL:      custom20 = {50015, 50016}; break;
}

if (bot->GetLevel() >= 20 && bot->GetLevel() < 40 && !custom20.empty())
{
    uint32 index = urand(0, custom20.size() - 1); 
    uint32 spell = custom20[index];
    bot->learnSpell(spell, false); 
    sLog.outDetail("Bot %d (level %d) learned custom level 20 mount spell %d",
    bot->GetGUIDLow(), bot->GetLevel(), spell);
}
#endif

// ==============================================================================
// Add in AutoLearnSpellAction.cpp - Custom Level 20 Mount Auto-Learn on Level-Up
// ==============================================================================

#ifdef MANGOSBOT_ZERO

std::vector<uint32> custom20;
switch (bot->getRace())
{
    case RACE_HUMAN:      custom20 = {34000, 34001}; break; // Honor Steed, Harvest Steed
    case RACE_ORC:        custom20 = {50003, 50004}; break;
    case RACE_DWARF:      custom20 = {50005, 50006}; break;
    case RACE_NIGHTELF:   custom20 = {50007, 50008}; break;
    case RACE_UNDEAD:     custom20 = {50009, 50010}; break;
    case RACE_TAUREN:     custom20 = {50011, 50012}; break;
    case RACE_GNOME:      custom20 = {50013, 50014}; break;
    case RACE_TROLL:      custom20 = {50015, 50016}; break;
}

uint32 firstmount = 40;

if (bot->GetLevel() >= 20 && bot->GetLevel() < firstmount && !custom20.empty())
{
    uint32 index = urand(0, custom20.size() - 1);
    uint32 spell = custom20[index];
    bot->learnSpell(spell, false);
    sLog.outDetail("Bot %d reached level %d and learned custom lvl20 mount spell %d",
    bot->GetGUIDLow(), bot->GetLevel(), spell);
}
#endif

 // =======================================================
 // Add in CheckMountStateAction.cpp - Allow Early Mounting
 // =======================================================

#ifdef MANGOSBOT_ZERO
// Allow early mounting if bot has class/custom quest movement spells
if (!firstmount)
{
    if (bot->HasSpell(34000) || // Honor Steed (human custom20_1)
        bot->HasSpell(34001) || // Harvest Steed (human custom20_2)
        bot->HasSpell(783) ||  // Travel Form (druid)
        bot->HasSpell(2645))   // Ghost Wolf (shaman)
    {
        firstmount = true;
        sLog.outDetail("Bot %d allowed early mount usage (custom20/travel/ghost form)", bot->GetGUIDLow());
    }
}
#endif
