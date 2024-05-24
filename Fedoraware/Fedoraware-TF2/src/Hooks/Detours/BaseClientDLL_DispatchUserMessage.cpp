#include "../Hooks.h"

#include "../../Features/Misc/Misc.h"
#include "../../Features/Logs/Logs.h"
#include"../../Features/NoSpread/NoSpreadHitscan/NoSpreadHitscan.h"

#include <boost/algorithm/string/replace.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include "../../Features/Visuals/Visuals.h"

static int anti_balance_attempts = 0;
static std::string previous_name;

static std::string clr({ '\x7', '0', 'D', '9', '2', 'F', 'F' });
static std::string yellow({ '\x7', 'C', '8', 'A', '9', '0', '0' }); //C8A900
static std::string white({ '\x7', 'F', 'F', 'F', 'F', 'F', 'F' }); //FFFFFF
static std::string green({ '\x7', '3', 'A', 'F', 'F', '4', 'D' }); //3AFF4D

MAKE_HOOK(BaseClientDLL_DispatchUserMessage, Utils::GetVFuncPtr(I::BaseClientDLL, 36), bool, __fastcall,
	void* ecx, void* edx, UserMessageType type, bf_read& msgData)
{
	const auto bufData = reinterpret_cast<const char*>(msgData.m_pData);
	msgData.SetAssertOnOverflow(false);

	F::Logs.UserMessage(type, msgData);
	msgData.Seek(0);

	switch (type)
	{
		case SayText2:
		{
			const int nbl = msgData.GetNumBytesLeft();
			if (nbl < 5 || nbl >= 256)
			{
				break;
			}

			const int entIdx = msgData.ReadByte();
			msgData.Seek(8);
			char typeBuffer[256], nameBuffer[256], msgBuffer[256];
			if (msgData.GetNumBytesLeft() == 0) { break; }
			msgData.ReadString(typeBuffer, 256);
			if (msgData.GetNumBytesLeft() == 0) { break; }
			msgData.ReadString(nameBuffer, 256);
			if (msgData.GetNumBytesLeft() == 0) { break; }
			msgData.ReadString(msgBuffer, 256);

			std::string chatType(typeBuffer);
			std::string playerName(nameBuffer);
			std::string chatMessage(msgBuffer);

			break;
		}

		case VoiceSubtitle:
		{
			int iEntityID = msgData.ReadByte();
			int iVoiceMenu = msgData.ReadByte();
			int iCommandID = msgData.ReadByte();

			if (iVoiceMenu == 1 && iCommandID == 6)
			{
				G::MedicCallers.push_back(iEntityID);
			}

			break;
		}

		case TextMsg:
		{
			if (F::NoSpreadHitscan.ParsePlayerPerf(msgData))
				return true;

			if (Vars::Misc::Automation::AntiAutobalance.Value && msgData.GetNumBitsLeft() > 35)
			{
				const INetChannel* server = I::EngineClient->GetNetChannelInfo();
				const std::string data(bufData);

				if (data.find("TeamChangeP") != std::string::npos && g_EntityCache.GetLocal())
				{
					const std::string serverName(server->GetAddress());
					if (serverName != previous_name)
					{
						previous_name = serverName;
						anti_balance_attempts = 0;
					}
					if (anti_balance_attempts < 2)
					{
						I::EngineClient->ClientCmd_Unrestricted("retry");
					}
				else
				{
					I::EngineClient->ClientCmd_Unrestricted(
						"tf_party_chat I will be autobalanced in 3 seconds");
				}
					anti_balance_attempts++;
				}
			}
			break;
		}

		case VGUIMenu:
		{
			// Remove MOTD
			if (Vars::Visuals::Removals::MOTD.Value)
			{
				if (strcmp(reinterpret_cast<char*>(msgData.m_pData), "info") == 0)
				{
					I::EngineClient->ClientCmd_Unrestricted("closedwelcomemenu");
					return true;
				}
			}

			break;
		}

		case ForcePlayerViewAngles:
		{
			return Vars::Visuals::Removals::AngleForcing.Value ? true : Hook.Original<FN>()(ecx, edx, type, msgData);
		}

		case SpawnFlyingBird:
		case PlayerGodRayEffect:
		case PlayerTauntSoundLoopStart:
		case PlayerTauntSoundLoopEnd:
		{
			return Vars::Visuals::Removals::Taunts.Value ? true : Hook.Original<FN>()(ecx, edx, type, msgData);
		}

		case Shake:
		case Fade:
		case Rumble:
		{
			return Vars::Visuals::Removals::ScreenEffects.Value ? true : Hook.Original<FN>()(ecx, edx, type, msgData);
		}
	}

	msgData.Seek(0);
	return Hook.Original<FN>()(ecx, edx, type, msgData);
}