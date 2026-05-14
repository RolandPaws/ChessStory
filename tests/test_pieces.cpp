#include "pieces/Piece.h"
#include "pieces/PieceType.h"
#include "player/Ability.h"
#include "player/Skill.h"
#include "player/Inventory.h"

#include <cassert>
#include <iostream>

#define TEST(name)                                    \
    do {                                              \
        std::cout << "  " << name << "... ";          \
        test_passed_ = true;                          \
    } while(0)

#define ASSERT(cond)                                  \
    do {                                              \
        if (!(cond)) {                                \
            std::cerr << "\nFAIL: " << #cond          \
                      << " (" << __FILE__ << ":"      \
                      << __LINE__ << ")\n";           \
            test_passed_ = false;                     \
        }                                             \
    } while(0)

#define END_TEST()                                    \
    do {                                              \
        std::cout << (test_passed_ ? "PASS" : "FAIL") << "\n"; \
        if (!test_passed_) ++failures;                \
    } while(0)

int main() {
    int failures = 0;
    bool test_passed_ = true;

    std::cout << "=== Piece Tests ===\n";

    // ── Piece creation and stats ──────────────────────────────────────────
    {
        TEST("King has correct base stats");
        Piece king(PieceType::King, Faction::Player);
        ASSERT(king.hp() == 10);
        ASSERT(king.attack() == 2);
        ASSERT(king.defence() == 1);
        ASSERT(king.moveRange() == 1);
        ASSERT(king.isAlive());
        END_TEST();
    }

    {
        TEST("Golem has high HP and defence");
        Piece golem(PieceType::Golem, Faction::Player);
        ASSERT(golem.hp() == 15);
        ASSERT(golem.attack() == 2);
        ASSERT(golem.defence() == 5);
        ASSERT(golem.moveRange() == 1);
        END_TEST();
    }

    {
        TEST("Phantom can phase through walls");
        Piece phantom(PieceType::Phantom, Faction::Player);
        ASSERT(phantom.canPhase());
        END_TEST();
    }

    {
        TEST("Valkyrie can fly");
        Piece valk(PieceType::Valkyrie, Faction::Player);
        ASSERT(valk.canFly());
        END_TEST();
    }

    // ── Damage and healing ────────────────────────────────────────────────
    {
        TEST("Take damage reduces HP");
        Piece pawn(PieceType::Pawn, Faction::Player);
        pawn.takeDamage(2);
        ASSERT(pawn.hp() == 2);   // 3 - max(1, 2-0) = 2
        END_TEST();
    }

    {
        TEST("Defence mitigates damage");
        Piece golem(PieceType::Golem, Faction::Player);
        golem.takeDamage(10);
        ASSERT(golem.hp() == 6);  // 15 - max(1, 10-5) = 6
        END_TEST();
    }

    {
        TEST("Healing restores HP");
        Piece pawn(PieceType::Pawn, Faction::Player);
        pawn.takeDamage(2);
        pawn.heal(1);
        ASSERT(pawn.hp() == 3);
        END_TEST();
    }

    {
        TEST("Heal does not exceed max HP");
        Piece pawn(PieceType::Pawn, Faction::Player);
        pawn.heal(100);
        ASSERT(pawn.hp() == pawn.maxHP());
        END_TEST();
    }

    {
        TEST("Death when HP reaches 0");
        Piece pawn(PieceType::Pawn, Faction::Player);
        pawn.takeDamage(100);
        ASSERT(!pawn.isAlive());
        ASSERT(pawn.hp() == 0);
        END_TEST();
    }

    // ── Level up ──────────────────────────────────────────────────────────
    {
        TEST("Level up increases stats");
        Piece pawn(PieceType::Pawn, Faction::Player);
        pawn.addXP(50);
        ASSERT(pawn.level() > 1);
        ASSERT(pawn.maxHP() > 3);
        ASSERT(pawn.attack() > 1);
        END_TEST();
    }

    // ── Abilities ─────────────────────────────────────────────────────────
    {
        TEST("Add and use ability");
        Piece mage(PieceType::Mage, Faction::Player);
        mage.addAbility(Ability("Test", "A test ability",
                                AbilityTrigger::Active, 1, 2,
                                [](Piece&) { return true; }));
        ASSERT(mage.abilities().size() == 1);
        ASSERT(mage.useAbility(0));  // should succeed
        END_TEST();
    }

    {
        TEST("Out of charges ability fails");
        Piece mage(PieceType::Mage, Faction::Player);
        mage.addAbility(Ability("OneShot", "Single use",
                                AbilityTrigger::Active, 5, 1,
                                [](Piece&) { return true; }));
        ASSERT(mage.useAbility(0));   // first use - succeeds
        ASSERT(!mage.useAbility(0));  // no charges left
        END_TEST();
    }

    // ── Inventory ─────────────────────────────────────────────────────────
    {
        TEST("Inventory add and remove items");
        Inventory inv;
        Item sword("Sword", "A sharp blade", Item::Category::Weapon,
                    Item::Rarity::Common, 2, 0, 0);
        ASSERT(inv.addItem(sword));
        ASSERT(inv.items().size() == 1);
        ASSERT(inv.removeItem(0));
        ASSERT(inv.items().empty());
        END_TEST();
    }

    {
        TEST("Inventory capacity limit");
        Inventory inv;
        for (size_t i = 0; i < Inventory::kMaxItems + 5; ++i) {
            inv.addItem(Item("Item", "desc", Item::Category::Consumable,
                              Item::Rarity::Common, 0, 0, 0));
        }
        ASSERT(inv.items().size() <= Inventory::kMaxItems);
        END_TEST();
    }

    std::cout << "\n" << (failures == 0 ? "All tests passed!" : "Some tests failed!")
              << " (" << failures << " failures)\n";
    return failures;
}
