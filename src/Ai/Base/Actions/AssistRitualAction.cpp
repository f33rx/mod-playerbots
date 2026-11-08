/*
 * This file is part of the mod-playerbots module for AzerothCore. See AUTHORS file for Copyright
 * information; released under GNU GPL v2 license, redistribute/modify under version 2 of the License,
 * or (at your option) any later version.
 */

#include "AssistRitualAction.h"
#include "CellImpl.h"
#include "GameObject.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Group.h"
#include "NearestGameObjects.h"
#include "Player.h"
#include "Playerbots.h"
#include "Spell.h"

namespace
{
constexpr float RITUAL_SEARCH_RANGE = 40.0f;

GameObject* FindGroupRitualPortal(Player* bot)
{
    if (!bot || !bot->GetGroup() || !bot->IsAlive() || bot->IsInFlight())
        return nullptr;

    std::list<GameObject*> gos;
    AnyGameObjectInObjectRangeCheck check(bot, RITUAL_SEARCH_RANGE);
    Acore::GameObjectListSearcher<AnyGameObjectInObjectRangeCheck> searcher(bot, gos, check);
    Cell::VisitObjects(bot, searcher, RITUAL_SEARCH_RANGE);

    for (GameObject* go : gos)
    {
        if (!go || !go->isSpawned() || go->GetGoType() != GAMEOBJECT_TYPE_SUMMONING_RITUAL)
            continue;

        GameObjectTemplate const* info = go->GetGOInfo();
        Unit* owner = go->GetOwner();
        if (!info || !owner || !owner->IsPlayer() || owner == bot)
            continue;
        if (!bot->IsInSameRaidWith(owner->ToPlayer()))
            continue;
        if (!owner->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
            continue;
        if (go->GetUniqueUseCount() >= info->summoningRitual.reqParticipants)
            continue;

        return go;
    }

    return nullptr;
}
}

bool RitualOfSummoningTrigger::IsActive()
{
    return FindGroupRitualPortal(bot) != nullptr;
}

GameObject* AssistRitualOfSummoningAction::FindPortal() const
{
    return FindGroupRitualPortal(bot);
}

bool AssistRitualOfSummoningAction::isUseful()
{
    return FindPortal() != nullptr;
}

bool AssistRitualOfSummoningAction::Execute(Event /*event*/)
{
    GameObject* portal = FindPortal();
    if (!portal)
        return false;

    if (bot->isMoving())
        bot->StopMoving();

    if (!portal->IsAtInteractDistance(bot))
        return MoveNear(portal, 3.0f);

    portal->Use(bot);
    return true;
}
