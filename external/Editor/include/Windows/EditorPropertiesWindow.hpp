#pragma once

#include "Core.hpp"

namespace wfe::editor {
    void LoadEditorProperties();
    void SaveEditorProperties();

    string GetDefaultTemplateLocation();
    void SetDefaultTemplateLocation(const string& newLocation);
    string GetDefaultProjectLocation();
    void SetDefaultProjectLocation(const string& newLocation);
    vector<string> GetTemplateLocations();
}