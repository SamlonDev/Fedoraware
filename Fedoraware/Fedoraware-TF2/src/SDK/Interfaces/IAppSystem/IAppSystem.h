#pragma once

#include <string>
#include <memory>
#include <typeindex>

class IAppSystem
{
public:
    virtual ~IAppSystem() = default;

    virtual bool Connect(CreateInterfaceFn factory) = 0;
    virtual void Disconnect() = 0;

    template <typename T>
    T* QueryInterface()
    {
        if (auto it = interfaces_.find(typeid(T)); it != interfaces_.end() && it->second == this)
        {
            return static_cast<T*>(it->first);
        }

        return nullptr;
    }

    virtual InitReturnVal Init() = 0;
    virtual void Shutdown() = 0;

protected:
    void AddInterface(const std::type_index& interfaceType, void* interface)
    {
        interfaces_[interfaceType] = interface;
    }

private:
    std::map<std::type_index, void*> interfaces_;
};

using IAppSystemPtr = std::unique_ptr<IAppSystem>;
