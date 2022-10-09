#pragma once

#include "WireframeEngineEditor.hpp"
#include "Base/Asset.hpp"

namespace wfe::editor {
    Asset* GetSelectedAsset();
    void SetSelectedAsset(Asset* newAsset);
}