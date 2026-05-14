#include "player/Skill.h"
#include "pieces/Piece.h"

Skill::Skill(std::string name, std::string description, Modifier mod, int32_t value)
    : name_(std::move(name))
    , description_(std::move(description))
    , modifier_(mod)
    , value_(value)
{
}

void Skill::applyTo(Piece& piece) const {
    switch (modifier_) {
    case Modifier::AttackBonus:
        piece.setAttack(piece.attack() + value_);
        break;
    case Modifier::DefenceBonus:
        piece.setDefence(piece.defence() + value_);
        break;
    case Modifier::HPBonus:
        // handled via max HP increase + heal
        break;
    case Modifier::MoveRangeBonus:
        piece.setMoveRange(piece.moveRange() + value_);
        break;
    case Modifier::Flying:
        // canFly flag is set via separate setter
        break;
    case Modifier::Phasing:
        // canPhase flag is set via separate setter
        break;
    default:
        break;
    }
}
