#pragma once
#include "Core.hpp"

namespace wfe {
    class Component;

    class System {
    public:
        typedef enum : uint32_t {
            CALLBACK_TYPE_START,
            CALLBACK_TYPE_UPDATE,
            CALLBACK_TYPE_BEFORE_RENDER,
            CALLBACK_TYPE_RENDER
        } CallbackType;

        typedef void(*Callback)(const vector<Component*>&, CallbackType);

        string name{};
        Callback callback{};

        System() = delete;
        System(const System&) = delete;
        System(System&&) noexcept = delete;
        System(const string& name, Callback callback);

        System& operator=(const System&) = delete;
        System& operator=(System&&) noexcept = delete;

        static vector<System*> GetSystems();
        static System* GetSystemWithName(const string& name);

        ~System();
    private:
        static set<System*> systems;
    };
}