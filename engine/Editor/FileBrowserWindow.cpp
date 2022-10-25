#include "Editor.hpp"
#include "ECS/EngineECS.hpp"
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

namespace wfe {
    // Constants
    const size_t FILE_BUTTON_SIZE = 90;
    const size_t FILE_BUTTON_BORDER = 5;

    // Variables
    string fileBrowserLocation = "";
    bool8_t renaming = false;
    string selectedLocation = "";

    // Internal helper functions
    static void RenderWindow() {
        auto& windowType = editor::WindowType::windowTypes->at("File Browser");

        ImGui::SetWindowSize(ImVec2(600.f, 300.f), ImGuiCond_FirstUseEver);
        if(ImGui::Begin("File Browser", &windowType.open)) {
            // Draw the top bar
            if(ImGui::Button("<") && fileBrowserLocation[0]) {
                char_t* lastSlash = fileBrowserLocation.data();

                for(char_t* ptr = fileBrowserLocation.data(); *(ptr + 1); ++ptr) {
                    if(*ptr == '/')
                        lastSlash = ptr;
                }
                *lastSlash = 0;
            }
            ImGui::SameLine();
            ImGui::TextUnformatted(fileBrowserLocation.c_str());
            
            // Calculate the required number of columns
            size_t windowWidth = (size_t)ImGui::GetWindowWidth();
            size_t columnCount = windowWidth / (FILE_BUTTON_SIZE + 2 * FILE_BUTTON_BORDER);
            if(!columnCount)
                columnCount = 1;

            // Draw every file/directory
            if(ImGui::BeginTable("FileTable", (int32_t)columnCount, ImGuiTableFlags_SizingFixedFit)) {
                ImGui::TableNextColumn();
                string location = editor::GetWorkspaceDir() + "assets/" + fileBrowserLocation;

                DIR* directory = opendir(location.c_str());

                size_t columnIndex = 0;
                size_t rowIndex = 0;
                ImGui::TableSetColumnIndex(columnIndex);

                dirent* entity = readdir(directory);
                while(entity) {
                    if(strcmp(entity->d_name, ".") && strcmp(entity->d_name, "..")) {
                        struct stat locationStat;
                        stat((location + entity->d_name).c_str(), &locationStat);

                        string fileLocation = fileBrowserLocation + entity->d_name;
                        ImVec4 backgroundColor;
                        if(selectedLocation == fileLocation || selectedLocation == fileLocation + "/")
                            backgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered);
                        else
                            backgroundColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);

                        if(S_ISREG(locationStat.st_mode)) {
                            if(ImGui::ImageButton(entity->d_name, editor::GetImGuiTexture(editor::IMAGE_ID_FILE), ImVec2(FILE_BUTTON_SIZE, FILE_BUTTON_SIZE), ImVec2(0, 0), ImVec2(1, 1), backgroundColor)) {
                                selectedLocation = fileLocation;

                                Asset* asset = Asset::GetAssetWithLocation(fileLocation);
                                if(asset)
                                    editor::SetSelectedAsset(asset);
                            }

                            string fileInfoPopupName = fileLocation + "FileInfo";
                            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                ImGui::OpenPopup(fileInfoPopupName.c_str());
                            }

                            if(ImGui::BeginPopup(fileInfoPopupName.c_str())) {
                                Asset* asset = Asset::GetAssetWithLocation(fileLocation);

                                if(ImGui::BeginMenu("Load as", !asset)) {
                                    for(auto& assetType : Asset::assetTypes) {
                                        if(ImGui::MenuItem(assetType.val2.name.c_str())) {
                                            Asset* loadedAsset = assetType.val2.create();
                                            loadedAsset->Load(fileLocation);
                                        }
                                    }

                                    ImGui::EndMenu();
                                }
                                if(ImGui::BeginMenu("Import as", !asset)) {
                                    for(auto& assetType : Asset::assetTypes) {
                                        if(ImGui::MenuItem(assetType.val2.name.c_str())) {
                                            Asset* importedAsset = assetType.val2.create();
                                            importedAsset->Import(fileLocation);
                                            importedAsset->Save(fileBrowserLocation + "New Asset.asset");
                                        }
                                    }

                                    ImGui::EndMenu();
                                }
                                if(ImGui::MenuItem("Rename")) {
                                    selectedLocation = fileLocation;
                                    renaming = true;
                                }
                                if(ImGui::MenuItem("Delete")) {
                                    if(asset) {
                                        delete asset;
                                        editor::SetSelectedAsset(nullptr);
                                    }
                                    
                                    remove((location + entity->d_name).c_str());
                                }

                                ImGui::EndPopup();
                            }
                            if(selectedLocation == fileLocation && ImGui::IsKeyPressed(ImGuiKey_F2)) {
                                renaming = true;
                            }
                            if(selectedLocation == fileLocation && ImGui::IsKeyPressed(ImGuiKey_Delete)) {
                                Asset* asset = Asset::GetAssetWithLocation(fileLocation);

                                if(asset) {
                                    delete asset;
                                    editor::SetSelectedAsset(nullptr);
                                }
                                
                                remove((location + entity->d_name).c_str());
                            }
                        } else if(S_ISDIR(locationStat.st_mode)) {
                            if(ImGui::ImageButton(entity->d_name, editor::GetImGuiTexture(editor::IMAGE_ID_FOLDER_EMPTY), ImVec2(FILE_BUTTON_SIZE, FILE_BUTTON_SIZE), ImVec2(0, 0), ImVec2(1, 1), backgroundColor)) {
                                if(selectedLocation == fileLocation + "/") {
                                    fileBrowserLocation = selectedLocation;
                                    selectedLocation = "";
                                    editor::SetSelectedAsset(nullptr);
                                    break;
                                } else
                                    selectedLocation = fileLocation + "/";
                                editor::SetSelectedAsset(nullptr);
                            }

                            string folderInfoPopupName = fileLocation + "FileInfo";
                            if(ImGui::IsItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right)) {
                                ImGui::OpenPopup(folderInfoPopupName.c_str());
                            }

                            if(ImGui::BeginPopup(folderInfoPopupName.c_str())) {
                                if(ImGui::MenuItem("Rename")) {
                                    selectedLocation = fileLocation;
                                    renaming = true;
                                }
                                if(ImGui::MenuItem("Delete"))
                                    remove((location + entity->d_name).c_str());

                                ImGui::EndPopup();
                            }
                            if(selectedLocation == fileLocation && ImGui::IsKeyPressed(ImGuiKey_F2)) {
                                renaming = true;
                            }
                            if(selectedLocation == fileLocation && ImGui::IsKeyPressed(ImGuiKey_Delete)) {
                                remove((location + entity->d_name).c_str());
                            }
                        }

                        if(renaming && (selectedLocation == fileLocation || selectedLocation == fileLocation + "/")) {
                            char_t nameBuffer[256];
                            strcpy(nameBuffer, entity->d_name);

                            ImGui::PushItemWidth(-.1f);
                            ImGui::InputText("", nameBuffer, 256);
                            if(ImGui::IsItemDeactivated() || ImGui::IsItemDeactivatedAfterEdit()) {
                                renaming = false;

                                if(selectedLocation[selectedLocation.length() - 1] == '/') {
                                    string newSelectedLocation = selectedLocation;
                                    char_t* lastSlash = newSelectedLocation.data() - 1;
                                    for(char_t* ptr = newSelectedLocation.data(); *(ptr + 1); ++ptr)
                                        if(*ptr == '/')
                                            lastSlash = ptr;
                                    
                                    strcpy(lastSlash + 1, nameBuffer);
                                    strcat(newSelectedLocation.data(), "/");

                                    for(auto* asset : Asset::GetAssets()) {
                                        if(strncmp(asset->location.c_str(), selectedLocation.c_str(), selectedLocation.length()))
                                            continue;
                                        
                                        char_t* newAssetLocation = new char_t[newSelectedLocation.length() + asset->location.length() - selectedLocation.length() + 1];
                                        strcpy(newAssetLocation, newSelectedLocation.c_str());
                                        strcat(newAssetLocation, asset->location.data() + selectedLocation.length());

                                        asset->location = newAssetLocation;
                                        delete[] newAssetLocation;
                                    }

                                    string oldLocation = editor::GetWorkspaceDir() + "assets/" + selectedLocation;
                                    string newLocation = editor::GetWorkspaceDir() + "assets/" + newSelectedLocation;
                                    rename((editor::GetWorkspaceDir() + "assets/" + selectedLocation).c_str(), (editor::GetWorkspaceDir() + "assets/" + newSelectedLocation).c_str());
                                    selectedLocation = newSelectedLocation;
                                } else {
                                    Asset* selectedAsset = editor::GetSelectedAsset();
                                    string newSelectedLocation = selectedLocation;
                                    char_t* lastSlash = newSelectedLocation.data() - 1;
                                    for(char_t* ptr = newSelectedLocation.data(); *ptr; ++ptr)
                                        if(*ptr == '/')
                                            lastSlash = ptr;
                                    
                                    strcpy(lastSlash + 1, nameBuffer);

                                    if(selectedAsset) {
                                        selectedAsset->name = nameBuffer;
                                        selectedAsset->location = newSelectedLocation;
                                    }

                                    string oldLocation = editor::GetWorkspaceDir() + "assets/" + selectedLocation;
                                    string newLocation = editor::GetWorkspaceDir() + "assets/" + newSelectedLocation;
                                    rename((editor::GetWorkspaceDir() + "assets/" + selectedLocation).c_str(), (editor::GetWorkspaceDir() + "assets/" + newSelectedLocation).c_str());
                                    selectedLocation = newSelectedLocation;
                                }
                            }
                            ImGui::PopItemWidth();
                        } else
                            ImGui::TextWrapped("%s", entity->d_name);

                        ++columnIndex;
                        if(columnIndex == columnCount) {
                            columnIndex = 0;
                            ++rowIndex;
                            ImGui::TableNextRow();
                        }
                        ImGui::TableSetColumnIndex(columnIndex);
                    }
                    entity = readdir(directory);
                }

                ImGui::EndTable();
            }

            if(ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left)) {
                selectedLocation = "";
                renaming = false;
                editor::SetSelectedAsset(nullptr);
            }
            if(ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Right))
                ImGui::OpenPopup("FileBrowserOptionsPopup");
            
            if(ImGui::BeginPopup("FileBrowserOptionsPopup")) {
                if(ImGui::BeginMenu("New asset")) {
                    for(auto& assetType : Asset::assetTypes)
                        if(ImGui::MenuItem(assetType.val2.name.c_str())) {
                            Asset* createdAsset = assetType.val2.create();
                            createdAsset->Save(fileBrowserLocation + "New asset.asset");
                        }

                    ImGui::EndMenu();
                }

                ImGui::EndPopup();
            }

            if(selectedLocation.length() && ImGui::IsKeyPressed(ImGuiKey_F2)) {
                renaming = true;
            }

            ImGui::End();
        }
    }

    WFE_EDITOR_WINDOW_TYPE("File Browser", RenderWindow)
}