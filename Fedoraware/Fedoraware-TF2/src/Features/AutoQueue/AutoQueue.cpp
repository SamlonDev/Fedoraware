#include "AutoQueue.h"

void CAutoQueue::Run()
{
	const bool bInGame = !I::EngineVGui->IsGameUIVisible() || I::EngineClient->IsInGame();

	// Auto queue
	if (Vars::Misc::Queueing::AutoCasualQueue.Value == 1)
	{
		if (bInGame)
			return;

		const bool bInStandbyQueue = I::TFPartyClient->BInStandbyQueue();
		const bool bHaveLiveMatch = I::TFGCClientSystem->BHaveLiveMatch();
		const int nNumMatchInvites = I::TFGCClientSystem->GetNumMatchInvites();

		if (!bInStandbyQueue && !bHaveLiveMatch && !nNumMatchInvites)
		{
			I::TFPartyClient->LoadSavedCasualCriteria();
			I::TFPartyClient->RequestQueueForMatch(k_eTFMatchGroup_Casual_Default);
		}
	}
	if (Vars::Misc::Queueing::AutoCasualQueue.Value == 2)
	{
		const bool bInQueueForMatchGroup = I::TFPartyClient->BInQueueForMatchGroup(k_eTFMatchGroup_Casual_Default);

		if (!bInQueueForMatchGroup)
		{
			I::TFPartyClient->LoadSavedCasualCriteria();
			I::TFPartyClient->RequestQueueForMatch(k_eTFMatchGroup_Casual_Default);
		}
	}
}
