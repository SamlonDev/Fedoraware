#pragma once
#include "KeyValues/KeyValues.h"

class CEconNotification
{
public:
    virtual ~CEconNotification() = default;
    virtual CEconNotification* CreateInstance() const = 0;

    virtual void SetLifetime(float flSeconds) = 0;
    virtual float GetExpireTime() const = 0;
    virtual float GetInGameLifeTime() const = 0;
    virtual bool BShowInGameElements() const = 0;
    virtual void MarkForDeletion() = 0;

    virtual EType NotificationType() const = 0;
    virtual bool BHighPriority() const = 0;
    virtual void Trigger() = 0;
    virtual void Accept() = 0;
    virtual void Decline() = 0;
    virtual void Deleted() = 0;
    virtual void Expired() = 0;
    virtual void UpdateTick() = 0;
    virtual const char* GetUnlocalizedHelpText() = 0;
    virtual void* CreateUIElement(bool bMainMenu) const = 0;

    const char* GetText() const;
    const char* GetSoundFilename() const;
    float GetExpireTimeSeconds() const;
    KeyValues* GetKeyValues() const;
    const wchar_t* GetBuffer() const;
    CSteamID GetSteamID() const;

    int GetID() const;
    bool IsInUse() const;

    void SetText(const char* pText);
    void SetSoundFilename(const char* pSoundFilename);
    void SetExpireTime(float flExpireTime);
    void SetKeyValues(KeyValues* pKeyValues);
    void SetSteamID(CSteamID steamID);

    void SetInUse(bool bInUse);

protected:
    CEconNotification() : m_iID(0), m_bInUse(false) {}

    const char* m_pText;
    const char* m_pSoundFilename;
    float m_flExpireTime;
    KeyValues* m_pKeyValues;
    wchar_t m_wszBuffer[1024];
    CSteamID m_steamID;

private:
    int m_iID;
    bool m_bInUse;

    friend class CEconNotificationQueue;
};
