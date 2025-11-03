// ==================================================================
// Add in PlayerbotFactory.cpp - Custom Level 20 Mount Initialization
// ==================================================================

#ifdef MANGOSBOT_ZERO

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
    case RACE_HUMAN:      custom20 = {50001, 50002}; break;
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

 // =============================================
 // Add in MountValues.cpp - Whitelist Spells
 // =============================================

bool MountValue::IsMountSpell(uint32 spellId)
{
    const SpellEntry* spellInfo = sSpellTemplate.LookupEntry<SpellEntry>(spellId);
    if (!spellInfo)
        return false;
  
    for (int i = 0; i < 3; ++i)
        if (spellInfo->EffectApplyAuraName[i] == SPELL_AURA_MOUNTED)
            return true;

    static const std::set<uint32> customMounts = {
        50001,50002,50003,50004,50005,50006,
        50007,50008,50009,50010,50011,50012,
        50013,50014,50015,50016
    };
    if (customMounts.find(spellId) != customMounts.end())
        return true;

    return false;
}
