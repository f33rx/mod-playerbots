/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#ifndef PLAYERBOTS_ASSISTRITUALACTION_H
#define PLAYERBOTS_ASSISTRITUALACTION_H

#include "MovementActions.h"
#include "Trigger.h"

class GameObject;

class RitualOfSummoningTrigger : public Trigger
{
public:
    RitualOfSummoningTrigger(PlayerbotAI* botAI) : Trigger(botAI, "ritual of summoning", 1) {}

    bool IsActive() override;
};

class AssistRitualOfSummoningAction : public MovementAction
{
public:
    AssistRitualOfSummoningAction(PlayerbotAI* botAI) : MovementAction(botAI, "assist ritual of summoning") {}

    bool Execute(Event event) override;
    bool isUseful() override;

private:
    GameObject* FindPortal() const;
};

#endif
