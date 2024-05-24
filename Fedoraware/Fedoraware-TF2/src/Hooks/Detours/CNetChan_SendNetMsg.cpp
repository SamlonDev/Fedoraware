#include "../Hooks.h"
#include "../../Features/TickHandler/TickHandler.h"

//	"NetMsg"
//	@net_chan.cpp L2524
MAKE_HOOK(CNetChan_SendNetMsg, S::CNetChan_SendNetMsg(), bool, __fastcall,
	CNetChannel* netChannel, void* edi, INetMessage& msg, bool bForceReliable, bool bVoice)
{
	/*
	bool retn = false, runfurther = false;
	if (Vars::NoSpread::Hitscan.Value) {
		if (F::NoSpread.SendNetMessage(&msg)) {
			bForceReliable = true;
			runfurther = true;
		}
	}
	*/

	switch (msg.GetType())
	{
		case clc_VoiceData:
		{
			// stop lag with voice chat
			bVoice = true;
			break;
		}

		case clc_FileCRCCheck:
		{
			// whitelist
			if (Vars::Misc::Exploits::BypassPure.Value)
			{
				return false;
			}
			break;
		}

		case clc_RespondCvarValue:
		{
			//	causes b1g crash
			if (Vars::Visuals::Removals::ConvarQueries.Value)
			{
				if (const auto respondMsg = reinterpret_cast<DWORD*>(&msg))
				{
					if (const auto cvarName = reinterpret_cast<const char*>(respondMsg[6]))
					{
						if (const auto convarC = g_ConVars.FindVar(cvarName))
						{
							if (const char* defaultValue = convarC->GetDefault())
							{
								respondMsg[7] = reinterpret_cast<DWORD>(defaultValue);
								I::Cvar->ConsoleColorPrintf({ 255, 0, 0, 255 }, "%s\n", msg.ToString()); //	mt everest
								break;
							}
						}
						return true; //	if we failed to manipulate the data, don't send it.
					}
				}
			}
			break;
		}

		case clc_Move:
		{
			static auto sv_maxusrcmdprocessticks = g_ConVars.FindVar("sv_maxusrcmdprocessticks");
			const int iTicks = sv_maxusrcmdprocessticks ? sv_maxusrcmdprocessticks->GetInt() : 24;
			const int iAllowedNewCommands = fmax(iTicks - G::ShiftedTicks, 0);
			const auto& moveMsg = reinterpret_cast<CLC_Move&>(msg);
			const int iCmdCount = moveMsg.m_nNewCommands + moveMsg.m_nBackupCommands - 3;
			if (iCmdCount > iAllowedNewCommands)
			{
				Utils::ConLog("clc_Move", std::format("{:d} sent <{:d} | {:d}>, max was {:d}.", iCmdCount, moveMsg.m_nNewCommands, moveMsg.m_nBackupCommands, iAllowedNewCommands).c_str(), { 0, 222, 255, 255 }, Vars::Debug::Logging.Value);
				G::ShiftedTicks = G::ShiftedGoal -= iCmdCount - iAllowedNewCommands;
				F::Ticks.iDeficit = iCmdCount - iAllowedNewCommands;
			}
			break;
		}
	}

	/*
	retn = Hook.Original<FN>()(netChannel, edi, msg, bForceReliable, bVoice);

	if (runfurther)
		F::NoSpread.SendNetMessagePost();

	return retn;
	*/

	return Hook.Original<FN>()(netChannel, edi, msg, bForceReliable, bVoice);
}