#pragma once

#include <memory>

class ICoreListener
{
public:
    virtual void OnCoreLoaded() = 0;
    virtual ~ICoreListener() {}
};

class CCore
{
    std::unique_ptr<ICoreListener> coreListener;

public:
    CCore(std::unique_ptr<ICoreListener> listener)
        : coreListener(std::move(listener))
    {
    }

    void Load()
    {
        // Perform loading logic here
        coreListener->OnCoreLoaded();
    }

    void Unload()
    {
        // Perform unloading logic here
    }

    bool ShouldUnload()
    {
        // Implement unload condition here
    }
};

inline CCore g_Core(std::make_unique<class CoreListenerImpl>());

class CoreListenerImpl : public ICoreListener
{
public:
    void OnCoreLoaded() override
    {
        // Handle the core loaded event here
    }
};
