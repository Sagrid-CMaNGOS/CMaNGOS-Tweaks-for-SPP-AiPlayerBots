// ==================================================================
// Add in PlayerbotFactory.cpp - Custom Level 20 Mount Initialization
// ==================================================================

#ifdef MANGOSBOT_ZERO

// Prepare a list of race-specific custom mounts

std::vector<uint32> custom20;
switch (bot->getRace())
{
    case RACE_HUMAN:      custom20 = {50001, 50002}; break;
    case RACE_ORC:        custom20 = {50003, 50004}; break;
    case RACE_DWARF:      custom20 = {50005, 50006}; break;
    case RACE_NIGHTELF:   custom20 = {50007, 50008}; break;  
    case RACE_UNDEAD:     custom20 = {50009, 50010}; break;
    case RACE_TAUREN:     custom20 = {50011, 50012}; break;
    case RACE_GNOME:      custom20 = {50013, 50014}; break;
    case RACE_TROLL:      custom20 = {50015, 50016}; break;
}

// Apply custom mounts to bots between level 20 and 39

if (bot->GetLevel() >= 20 && bot->GetLevel() < 40 && !custom20.empty())
{
    uint32 index = urand(0, custom20.size() - 1); // Pick a random mount
    uint32 spell = custom20[index];
    bot->learnSpell(spell, false); // Teach the bot the mount spell
    sLog.outDetail("Bot %d (level %d) learned custom level 20 mount spell %d",
    bot->GetGUIDLow(), bot->GetLevel(), spell);

    return; // Stop further mount initialization for lvl20 bots
}
#endif

// ==============================================================================
// Add in AutoLearnSpellAction.cpp - Custom Level 20 Mount Auto-Learn on Level-Up
// ==============================================================================

#ifdef MANGOSBOT_ZERO

// Prepare a list of race-specific custom mounts

std::vector<uint32> custom20;
switch (bot->getRace())
{
    case RACE_HUMAN:      custom20 = {50001, 50002}; break;
    case RACE_ORC:        custom20 = {50003, 50004}; break;
    case RACE_DWARF:      custom20 = {50005, 50006}; break;
    case RACE_NIGHTELF:   custom20 = {50007, 50008}; break;
    case RACE_UNDEAD:     custom20 = {50009, 50010}; break;
    case RACE_TAUREN:     custom20 = {50011, 50012}; break;
    case RACE_GNOME:      custom20 = {50013, 50014}; break;
    case RACE_TROLL:      custom20 = {50015, 50016}; break;
}

// Threshold for standard "slow" mount (lvl40)

uint32 firstmount = 40;

// Learn lvl20 mount automatically if bot levels up >=20 but < firstmount

if (bot->GetLevel() >= 20 && bot->GetLevel() < firstmount && !custom20.empty())
{
    uint32 index = urand(0, custom20.size() - 1); // Pick a random mount
    uint32 spell = custom20[index];
    bot->learnSpell(spell, false); // Teach the bot the mount spell
    sLog.outDetail("Bot %d reached level %d and learned custom lvl20 mount spell %d",
    bot->GetGUIDLow(), bot->GetLevel(), spell);
}
#endif

// ======================================================
// Add in CheckMountStateAction.cpp - Guard for Level <20
// ======================================================

#ifdef MANGOSBOT_ZERO

// Prevent bots below level 20 from mounting

if (bot->GetLevel() < 20)
{

// Optional debug message
  
if (requester && ai->HasStrategy("debug mount", BotState::BOT_STATE_NON_COMBAT))
      ai->TellPlayerNoFacing(requester, "Too low level to mount (requires level 20).");

    return false; // Prevent mount action
}
#endif
