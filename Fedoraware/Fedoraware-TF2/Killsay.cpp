#include "Killsay.h"

#include <boost/algorithm/string/replace.hpp>
#include "../src/SDK/Includes/Enums.h"
#include "../src/SDK/Main/BaseEntity/BaseEntity.h"
#include <vector>
#include <string>

// Initialize the killsay list
std::vector<std::string> killsays = {
    "%victim%, you've just been media'ed",
    "%victim%, H - Harder Sempai!",
    "This is a dream, i cant believe im putting my dick in %victim% !",
    "%victim%, i thought rijin is god cheat",
    "%victim% you've just been tapped by salmonpaste",
    "SalmonPaste, superior to other cheats, as experienced by %victim%",
    "%victim% how u lost to free money bot aka fware ?",
    "is %victim% really giving me a titjob ?",
    "Inferior % victim % got dommed by gigachad SalmonPaste user",
    "%victim % Lost to a paste, what a noob!!",
    "%victim% Sucks!get gud get salmonpaste",
    "%victim% You should behead yourself, tehee",
    "%victim%, shit paste detected, kill rejected",
    "%victim% if you dont wanna get tapped, join https://discord.gg/XFJbKC2C2P",
    "%victim% Got dominated by SalmonPaste",
    "My dear %victim% don't trashtalk too much",
    "%victim% how dare you even think of your shit paste going head to head with salmonpaste",
    "I Guess making %victim% angry makes her wet too.",
    "DECAPITATED, GET GUD% victim%",
    "%victim% Lost HvH to a paste smh",
    "DECIMATED get better bozo!",
    "I dickride salmonpaste and you should too %victim%!",
    "1, GET RAPED BY SALMONPASTE",
    "crackhead 0wned 144hz 0ms",
    "inferior cheat smh",
    "I fucking really wanna know how you're so bad at this game, %victim%",
    "%victim% I'm surprised you managed to hit the install button",
    "niejes cienszko",
    "Another clueless nn Destroyed",
    "Why are you even trying to HvH someone with SalmonPaste anyways, %victim%",
    "Get shit on by SalmonPaste, %victim%",
    "Why dont you sit down like a good dog, %victim%",
    "%victim% touch grass, please...",
    "%victim% are you even trying at this point?",
    "brainrot 0wnage at this point, %victim%...",
    "%victim% imagine not using SalmonPaste :skull:",
    "%victim%'s paste 0-1 SalmonPaste",
    "self pasted, self owned",
    "Your paste has been denied by SalmonDev",
    "what a fucking retard ~ Terry A. Davis to %victim%",
    "who set the bots to passive?",
    "24.05.2024, trashtalk has been added",
    "%victim% I'd call you cancer, but at least cancer gets kills",
    "you sperg, i own as simple as that."
};

void CKillsay::FireGameEvent(FNV1A_t uNameHash, CGameEvent* pEvent)
{
    if (!Vars::Misc::Paste::Krillsays.Value)
    {
        return;
    }

    if (killsays.empty())
    {
        return;
    }

    if (uNameHash == FNV1A::HashConst("player_death"))
    {
        const int attacker = Utils::GetPlayerForUserID(pEvent->GetInt("attacker"));
        const int userid = Utils::GetPlayerForUserID(pEvent->GetInt("userid"));

        if (userid == I::EngineClient->GetLocalPlayer())
        {
            return;
        }

        if (attacker != I::EngineClient->GetLocalPlayer())
        {
            return;
        }

        // Lol lets hope u dont have medal flip on and 9 lines of killsay

        const auto& pEntity = I::ClientEntityList->GetClientEntity(userid);

        if (!pEntity)
        {
            return;
        }

        if (!pEntity->IsPlayer())
        {
            return;
        }

        PlayerInfo_t pi = {};

        if (!I::EngineClient->GetPlayerInfo(userid, &pi))
        {
            return;
        }

        int nSelectedKillsay = I::UniformRandomStream->RandomInt(0, killsays.size() - 1);
        std::string selectedKillsay = killsays.at(nSelectedKillsay);

        const char* szName = pi.name;
        boost::replace_all(selectedKillsay, "%victim%", szName);

        const std::string cmd = "say \"" + selectedKillsay + "\"";
        I::EngineClient->ServerCmd(cmd.c_str(), true);
    }
}
