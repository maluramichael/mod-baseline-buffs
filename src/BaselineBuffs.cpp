/*
 * mod-baseline-buffs
 *
 * Grants three class utility buffs as baseline (learnable without spending a talent point):
 *   - Priest: Divine Spirit (normally a Discipline talent)
 *   - Paladin: Blessing of Kings (normally a Protection talent)
 *   - Druid: Mark of the Wild (ensure the best rank is available)
 *
 * Uses C++ PlayerScript to grant the appropriate spell rank on login and on level-up.
 * Grants the HIGHEST rank whose required level <= player level.
 *
 * This module works for players AND playerbots (no trainer visit needed).
 *
 * Released under GNU GPL v2 or (at your option) any later version.
 */

#include "Config.h"
#include "Player.h"
#include "ScriptMgr.h"

#include <iterator>

namespace BaselineBuffs
{
    // Cached config (populated in WorldScript::OnAfterConfigLoad).
    struct Config
    {
        bool Enable  = true;
        bool Priest  = true;
        bool Paladin = true;
        bool Druid   = true;
    };

    Config& GetConfig()
    {
        static Config cfg;
        return cfg;
    }

    // Spell rank table: {classId, minLevel, spellId}
    // WotLK 3.3.5a (build 12340)
    struct SpellRank
    {
        uint8  classId;
        uint8  minLevel;
        uint32 spellId;
    };

    // Blessing of Kings (Paladin)
    // Single spell entry, scaling with level
    static const SpellRank BLESSING_OF_KINGS[] = {
        { 2, 20, 20217 },  // Blessing of Kings
    };

    // Divine Spirit (Priest)
    // Multiple ranks, each requires a specific level
    static const SpellRank DIVINE_SPIRIT[] = {
        { 5, 30, 14752 },  // Divine Spirit (Rank 1, level 30)
        { 5, 40, 14818 },  // Divine Spirit (Rank 2, level 40)
        { 5, 50, 14819 },  // Divine Spirit (Rank 3, level 50)
        { 5, 60, 27841 },  // Divine Spirit (Rank 4, level 60)
        { 5, 69, 25312 },  // Divine Spirit (Rank 5, level 69)
        { 5, 79, 48073 },  // Divine Spirit (Rank 6, level 79)
    };

    // Mark of the Wild (Druid)
    // Multiple ranks, each requires a specific level
    static const SpellRank MARK_OF_THE_WILD[] = {
        { 11, 10, 1126  },  // Mark of the Wild (Rank 1, level 10)
        { 11, 20, 5232  },  // Mark of the Wild (Rank 2, level 20)
        { 11, 30, 6756  },  // Mark of the Wild (Rank 3, level 30)
        { 11, 40, 5234  },  // Mark of the Wild (Rank 4, level 40)
        { 11, 50, 8907  },  // Mark of the Wild (Rank 5, level 50)
        { 11, 60, 9884  },  // Mark of the Wild (Rank 6, level 60)
        { 11, 70, 9885  },  // Mark of the Wild (Rank 7, level 70)
        { 11, 71, 26990 },  // Mark of the Wild (Rank 8, level 71)
        { 11, 75, 48469 },  // Mark of the Wild (Rank 9, level 75)
    };

    // Helper: learn all spells from a rank table that the player qualifies for.
    void LearnSpellsFromTable(Player* player, SpellRank const* table, size_t tableSize)
    {
        if (!player)
            return;

        uint8 playerLevel = player->GetLevel();

        // Iterate the table and learn every spell whose minLevel <= playerLevel
        for (size_t i = 0; i < tableSize; ++i)
        {
            SpellRank const& rank = table[i];

            // Only teach if the player's level meets the requirement
            if (playerLevel >= rank.minLevel)
            {
                // Only teach if not already known
                if (!player->HasSpell(rank.spellId))
                {
                    player->learnSpell(rank.spellId, false);
                }
            }
        }
    }
}

using BaselineBuffs::GetConfig;
using BaselineBuffs::LearnSpellsFromTable;

// =====================================================================
//  PlayerScript: grant buffs on login and level-up.
// =====================================================================
class BaselineBuffsPlayerScript : public PlayerScript
{
public:
    BaselineBuffsPlayerScript() : PlayerScript("BaselineBuffs_PlayerScript") { }

    void OnPlayerLogin(Player* player) override
    {
        BaselineBuffs::Config const& cfg = GetConfig();
        if (!cfg.Enable)
            return;

        if (!player)
            return;

        uint8 playerClass = player->getClass();

        // Grant Priest buffs (Divine Spirit)
        if (cfg.Priest && playerClass == CLASS_PRIEST)
        {
            LearnSpellsFromTable(player, BaselineBuffs::DIVINE_SPIRIT,
                                 std::size(BaselineBuffs::DIVINE_SPIRIT));
        }

        // Grant Paladin buffs (Blessing of Kings)
        if (cfg.Paladin && playerClass == CLASS_PALADIN)
        {
            LearnSpellsFromTable(player, BaselineBuffs::BLESSING_OF_KINGS,
                                 std::size(BaselineBuffs::BLESSING_OF_KINGS));
        }

        // Grant Druid buffs (Mark of the Wild)
        if (cfg.Druid && playerClass == CLASS_DRUID)
        {
            LearnSpellsFromTable(player, BaselineBuffs::MARK_OF_THE_WILD,
                                 std::size(BaselineBuffs::MARK_OF_THE_WILD));
        }
    }

    void OnPlayerLevelChanged(Player* player, uint8 /*oldLevel*/) override
    {
        BaselineBuffs::Config const& cfg = GetConfig();
        if (!cfg.Enable)
            return;

        if (!player)
            return;

        uint8 playerClass = player->getClass();

        // Grant Priest buffs (Divine Spirit)
        if (cfg.Priest && playerClass == CLASS_PRIEST)
        {
            LearnSpellsFromTable(player, BaselineBuffs::DIVINE_SPIRIT,
                                 std::size(BaselineBuffs::DIVINE_SPIRIT));
        }

        // Grant Paladin buffs (Blessing of Kings)
        if (cfg.Paladin && playerClass == CLASS_PALADIN)
        {
            LearnSpellsFromTable(player, BaselineBuffs::BLESSING_OF_KINGS,
                                 std::size(BaselineBuffs::BLESSING_OF_KINGS));
        }

        // Grant Druid buffs (Mark of the Wild)
        if (cfg.Druid && playerClass == CLASS_DRUID)
        {
            LearnSpellsFromTable(player, BaselineBuffs::MARK_OF_THE_WILD,
                                 std::size(BaselineBuffs::MARK_OF_THE_WILD));
        }
    }
};

// =====================================================================
//  WorldScript: config load.
// =====================================================================
class BaselineBuffsWorldScript : public WorldScript
{
public:
    BaselineBuffsWorldScript() : WorldScript("BaselineBuffs_WorldScript") { }

    void OnAfterConfigLoad(bool /*reload*/) override
    {
        BaselineBuffs::Config& cfg = GetConfig();
        cfg.Enable  = sConfigMgr->GetOption<bool>("BaselineBuffs.Enable", true);
        cfg.Priest  = sConfigMgr->GetOption<bool>("BaselineBuffs.Priest", true);
        cfg.Paladin = sConfigMgr->GetOption<bool>("BaselineBuffs.Paladin", true);
        cfg.Druid   = sConfigMgr->GetOption<bool>("BaselineBuffs.Druid", true);
    }
};

// =====================================================================
//  Registration
// =====================================================================
void AddBaselineBuffsScripts()
{
    new BaselineBuffsPlayerScript();
    new BaselineBuffsWorldScript();
}
