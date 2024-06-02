#include "../Hooks.h"

#include "../../Features/Misc/Misc.h"
#include "../../Features/Logs/Logs.h"
#include "../../Features/NoSpread/NoSpreadHitscan/NoSpreadHitscan.h"
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
			if (msgData.GetNumBytesLeft() < 3 * 256) { break; }
			msgData.ReadString(typeBuffer, sizeof(typeBuffer));
			if (msgData.GetNumBytesLeft() < 256) { break; }
			msgData.ReadString(nameBuffer, sizeof(nameBuffer));
			if (msgData.GetNumBytesLeft() < 256) { break; }
			msgData.ReadString(msgBuffer, sizeof(msgBuffer));

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
				I
