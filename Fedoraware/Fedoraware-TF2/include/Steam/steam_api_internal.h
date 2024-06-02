#ifndef STEAMWORKS_COMMON_H
#define STEAMWORKS_COMMON_H

#include <memory>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <optional>
#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <stdexcept>

namespace Steamworks
{

class ISteamClient;
class ISteamUser;
class ISteamFriends;
class ISteamUtils;
class ISteamMatchmaking;
class ISteamContentServer;
class ISteamMatchmakingServers;
class ISteamUserStats;
class ISteamApps;
class ISteamNetworking;
class ISteamRemoteStorage;
class ISteamScreenshots;
class ISteamMusic;
class ISteamMusicRemote;
class ISteamGameServerStats;
class ISteamPS3OverlayRender;
class ISteamHTTP;
class ISteamController;
class ISteamUGC;
class ISteamAppList;
class ISteamHTMLSurface;
class ISteamInventory;
class ISteamVideo;
class ISteamParentalSettings;
class ISteamGameSearch;
class ISteamInput;
class ISteamParties;
class ISteamRemotePlay;

class CCallbackBase;
class CCallResult;
class CCallback;

class CSteamAPIContext;
class CSteamGameServerAPIContext;

} // namespace Steamworks

#endif // STEAMWORKS_COMMON_H

#ifndef STEAMWORKS_CALLBACK_H
#define STEAMWORKS_CALLBACK_H

#include <functional>
#include <utility>

namespace Steamworks
{

class CCallbackBase
{
public:
    enum class ECallbackFlags
    {
        k_ECallbackFlagsNone = 0,
        k_ECallbackFlagsRegistered = (1 << 0),
        k_ECallbackFlagsDeleted = (1 << 1),
    };

    CCallbackBase();
    virtual ~CCallbackBase();

    CCallbackBase(const CCallbackBase&) = delete;
    CCallbackBase& operator=(const CCallbackBase&) = delete;

    virtual void Run(void* pvParam) = 0;

    ECallbackFlags GetFlags() const;
    void SetFlags(ECallbackFlags flags);

protected:
    std::atomic<ECallbackFlags> m_Flags;
};

class CCallResult
{
public:
    CCallResult();
    CCallResult(CCallResult&&) noexcept;
    CCallResult& operator=(CCallResult&&) noexcept;
    ~CCallResult();

    CCallResult(const CCallResult&) = delete;
    CCallResult& operator=(const CCallResult&) = delete;

    void Set(SteamAPICall_t hAPICall, std::function<void(P*)> func);
    bool IsActive() const;
    void Cancel();

    template <typename T, typename P>
    friend class CCallback;

private:
    SteamAPICall_t m_hAPICall;
    std::function<void(P*)> m_Func;
};

class CCallback
{
public:
    CCallback(T* pObj, std::function<void(T*, P*)> func);
    CCallback(T* pObj, std::function<void(T*, P*, bool)> func);
    CCallback(T* pObj, std::function<void(T*, P*, bool, SteamAPICall_t)> func);
    CCallback(const CCallback&) = delete;
    CCallback& operator=(const CCallback&) = delete;
    ~CCallback();

    void Register();
    void Unregister();

    template <typename P>
    void Run(P* pParam);

private:
    T* m_pObj;
    std::function<void(T*, P*)> m_Func;
    std::unique_ptr<CCallbackBase> m_pCallback;
};

} // namespace Steamworks

#endif // STEAMWORKS_CALLBACK_H

#ifndef STEAMWORKS_API_H
#define STEAMWORKS_API_H

#include <memory>
#include <string>
#include <array>
#include <vector>
#include <unordered_map>
#include <optional>
#include <atomic>
#include <chrono>
#include <thread>
#include <mutex>
#include <functional>
#include <stdexcept>

namespace Steamworks
{

class ISteamClient
{
public:
    virtual HSteamPipe GetHSteamPipe() = 0;
    virtual HSteamUser GetHSteamUser() = 0;
};

class ISteamUser
{
public:
    virtual HSteamPipe GetHSteamPipe() = 0;
    virtual HSteamUser GetHSteamUser() = 0;
};

class ISteamGameServer
{
public:
    virtual HSteamPipe GetHSteamPipe() = 0;
    virtual HSteamUser GetHSteamUser() = 0;
};

class ISteamUtils
{
public:
    virtual void GetIPCountry(uint32 unIP, char* pCountry, int cchCountryBufferSize) = 0;
};

class ISteamMatchmaking
{
public:
    virtual bool RequestEncryptedAppTicket(uint32 unSize, void* pTicket) = 0;
};

class ISteamContentServer
{
public:
    virtual bool BIsSubscribed(const char* pchAppID) = 0;
};

class ISteamMatchmakingServers
{
public:
    virtual bool GetServerDetails(uint32 unServer, ServerDetails_t& details) = 0;
};

class ISteamUserStats

