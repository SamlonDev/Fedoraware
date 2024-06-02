#include "AutoQueue.h"

void CAutoQueue::Run()
{
    // Check if the game is in progress
    const bool bInGame = !I::EngineVGui->IsGameUIVisible() || I::EngineClient->IsInGame();
    if (bInGame)
    {
        return;
    }

    // Get the current auto-queue setting
    const int autoQueueSetting = Vars::Misc::Queueing::AutoCasualQueue.Value;
    if (autoQueueSetting != 1 && autoQueueSetting != 2)
    {
        // The auto-queue setting is not valid, so do nothing
        return;
    }

    // Check if the player is in the standby queue or has a live match
    IPartyClient *partyClient = I::TFPartyClient;
    if (partyClient != nullptr)
    {
        const bool bInStandbyQueue = partyClient->BInStandbyQueue();
        const bool bHaveLiveMatch = I::TFGCClientSystem != nullptr && I::TFGCClientSystem->BHaveLiveMatch();

        if (!bInStandbyQueue && !bHaveLiveMatch)
        {
            // Check if there are any match invites
            const int nNumMatchInvites = I::TFGCClientSystem != nullptr ? I::TFGCClientSystem->GetNumMatchInvites() : 0;
            if (nNumMatchInvites == 0)
            {
                // The player is not in the standby queue, does not have a live match, and does not have any match invites
                // So queue for a casual match
                partyClient->LoadSavedCasualCriteria();
                partyClient->RequestQueueForMatch(k_eTFMatchGroup_Casual_Default);
            }
        }
    }

    // Check if the player should queue for a casual match based on the auto-queue setting
    if (autoQueueSetting == 1)
    {
        // The player should queue for a casual match if they are not already in the queue
        IPartyClient *partyClient = I::TFPartyClient;
        if (partyClient != nullptr)
        {
            const bool bInQueueForMatchGroup = partyClient->BInQueueForMatchGroup(k_eTFMatchGroup_Casual_Default);
            if (!bInQueueForMatchGroup)
            {
                partyClient->LoadSavedCasualCriteria();
                partyClient->RequestQueueForMatch(k_eTFMatchGroup_Casual_Default);
            }
        }
    }
}
