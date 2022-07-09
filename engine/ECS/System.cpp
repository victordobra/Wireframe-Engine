#include "System.hpp"

namespace wfe {
    // Static variables
    set<System*> System::systems;

    // Member functions
    System::System(const string& name, Callback callback) : name(name), callback(callback) {
        systems.insert(this);
    }

    System::~System() {
        systems.erase(this);
    }

    // Static functions
    vector<System*> System::GetSystems() {
        return { systems.begin(), systems.end() };
    }
    System* System::GetSystemWithName(const string& name) {
        for(auto* system : systems)
            if(system->name == name)
                return system;
        return nullptr;
    }
}