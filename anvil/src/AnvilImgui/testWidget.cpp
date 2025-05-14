#include "testWidget.h"

#include <fstream>
#include "../Logger/Logger.h"

namespace anvil {

testWidget::testWidget() {
    rootGroup.id = "root";
    rootGroup.dependencies = "";
    rootGroup.actionAfterComplete = "";
}

void testWidget::draw() {
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (restaurantIsChosenForEditing) {
        DrawQuestEditorWidget();
    }
    else {
        DrawRestaurantsList();
    }
    for (auto& cb : deferredCallbacks) {
        cb();
    }
    deferredCallbacks.clear();
}

void testWidget::DrawQuestEditorWidget() {
    ImGui::Begin("Quest Editor");

    ImGui::Columns(2, nullptr, true);
    ImGui::BeginChild("LeftTree", ImVec2(0, -70), true);
    if (!restaurantIsChosenForEditing) {
        ImGui::TextColored(ImVec4(1, 0.5f, 0.2f, 1), "Please choose restaurant first.");
        ImGui::EndChild();
        ImGui::NextColumn();
        ImGui::End();
        return;
    }
    bool canEditTree = !restaurantName.empty() && startId > 0;
    if (!canEditTree) {
        ImGui::TextColored(ImVec4(1, 0.5f, 0.2f, 1), "Please enter restaurant's name and startId > 0 first.");
    }

    groupIdCounter = 0;
    questIdCounter = 1;
    drawGroupTree(rootGroup, groupIdCounter, questIdCounter);

    ImGui::EndChild();


    if (canEditTree) {
        drawTreeButtons();
    }

    ImGui::NextColumn();
    drawRightPanel();

    ImGui::End();
}

void testWidget::drawTreeButtons() {
    if (ImGui::Button("add quest")) {
        int gIndex = 0;
        QuestGroup* target = findGroupByIndex(rootGroup, gIndex, selectedGroupId);
        if (target) {
            int index = getNextQuestIndex();
            std::string newQuestId = std::to_string(startId + index);
            target->quests.push_back({newQuestId, "hardSetup", ""});
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("add group")) {
        int gIndex = 0;
        QuestGroup* target = findGroupByIndex(rootGroup, gIndex, selectedGroupId);
        if (target) {
            int index = getNextGroupIndex();
            std::string newGroupId = std::to_string(index);
            target->subGroups.push_back(std::make_unique<QuestGroup>(QuestGroup{
                newGroupId, "", "", {}
            }));
        }
    }
    if (ImGui::Button("delete selected")) {
        if (selectedQuestId != -1) {
            int qIndex = 1;
            if (removeQuestByIndex(rootGroup, qIndex, selectedQuestId)) {
                selectedQuestId = -1;
            }
        } else if (selectedGroupId != -1) {
            int gIndex = 0;
            if (removeGroupByIndex(rootGroup, gIndex, selectedGroupId)) {
                selectedGroupId = -1;
            }
        }
    }
    ImGui::SameLine();
    if (ImGui::Button("clear")) {
        rootGroup.subGroups.clear();
        rootGroup.quests.clear();
        selectedGroupId = 0;
        selectedQuestId = -1;
        groupIdCounter = 0;
        questIdCounter = 1;
        freeGroupIndices.clear();
        freeQuestIndices.clear();
    }

    ImGui::SameLine();
    if (ImGui::Button("save and reload")) {
        std::filesystem::path path = std::filesystem::path(ftmPath) / "assets/generic/properties/quests" / (restaurantName + "Cafe.json");
        saveQuestTreeToJson(path);
    }
}
    
void testWidget::drawGroupTree(QuestGroup& group, int& groupCounter, int& questCounter) {
    int thisGroupIndex = groupCounter;
    bool isRoot = group.id == "root";
    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_DefaultOpen;
    if (selectedGroupId == thisGroupIndex && selectedQuestId == -1) {
        flags |= ImGuiTreeNodeFlags_Selected;
    }

    ImGui::SetNextItemOpen(true, ImGuiCond_Always);
    bool open = ImGui::TreeNodeEx(
        (restaurantName + "_group" + group.id + "##group" + std::to_string(thisGroupIndex)).c_str(),
        flags
    );

    if (ImGui::IsItemClicked()) {
        selectedGroupId = groupCounter;
        selectedQuestId = -1;
    }

    if (open) {
        for (size_t i = 0; i < group.quests.size(); ++i) {
            int thisQuestIndex = questCounter;
            Quest& quest = group.quests[i];
            std::string label = quest.id + "##quest" + std::to_string(questCounter);
            if (ImGui::Selectable(label.c_str(), selectedQuestId == questCounter)) {
                selectedGroupId = thisGroupIndex;
                selectedQuestId = thisQuestIndex;
            }
            ++questCounter;
        }

        for (auto& sub : group.subGroups) {
            ++groupCounter;
            drawGroupTree(*sub, groupCounter, questCounter);
        }

        ImGui::TreePop();
    }
}

void testWidget::drawRightPanel() {
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    if (!regionsLoaded) {
        ImGui::Text("Load JSON file to see restaurants.");
        ImGui::EndChild();
        ImGui::End();
        return;
    }

    if (!restaurantIsChosenForEditing) {
        DrawRestaurantsList();
        return;
    }

    if (selectedQuestId != -1) {
        int qIndex = 1;
        Quest* quest = findQuestByIndex(rootGroup, qIndex, selectedQuestId);
        if (quest) {
            char idBuf[128], decoBuf[128];
            std::strncpy(idBuf, quest->id.c_str(), sizeof(idBuf));
            std::strncpy(decoBuf, quest->affectedDecoration.c_str(), sizeof(decoBuf));

            if (ImGui::InputText("quest ID", idBuf, sizeof(idBuf))) quest->id = idBuf;

            const char* mainActionTypes[] = { "decorationAction", "customAction" };
            int typeIdx = (quest->mainActionType == "decorationAction") ? 0 : 1;
            if (ImGui::Combo("Main action type", &typeIdx, mainActionTypes, IM_ARRAYSIZE(mainActionTypes))) {
                quest->mainActionType = mainActionTypes[typeIdx];
            }
            if (quest->mainActionType == "customAction") {
                bool controlPoints = quest->controlPoints;
                if (ImGui::Checkbox("Control points", &controlPoints)) {
                    quest->controlPoints = controlPoints;
                }
            }
            else if (quest->mainActionType == "decorationAction") {
                const char* decorationActionTypes[] = { "variableSetup", "hardSetup" };
                int decoIdx = (quest->decorationActionType == "variableSetup") ? 0 : 1;
                if (ImGui::Combo("Decoration action type", &decoIdx, decorationActionTypes, IM_ARRAYSIZE(decorationActionTypes))) {
                    quest->decorationActionType = decorationActionTypes[decoIdx];
                }
                if (quest->decorationActionType == "hardSetup") {
                    char decorationIdBuf[128], skinIdBuf[128] = "";
                    strncpy(decorationIdBuf, quest->affectedDecoration.c_str(), sizeof(decorationIdBuf));
                    strncpy(skinIdBuf, quest->decorationSkin.c_str(), sizeof(skinIdBuf));
                    if (ImGui::InputText("decoration", decorationIdBuf, sizeof(decorationIdBuf))) {
                        quest->affectedDecoration = decorationIdBuf;
                    }
                    if (ImGui::InputText("skin", skinIdBuf, sizeof(skinIdBuf))) {
                        quest->decorationSkin = skinIdBuf;
                    }
                }
                else if (quest->decorationActionType == "variableSetup") {
                    char decoBuf[128];
                    strncpy(decoBuf, quest->affectedDecoration.c_str(), sizeof(decoBuf));
                    if (ImGui::InputText("affected decoration", decoBuf, sizeof(decoBuf))) {
                        quest->affectedDecoration = decoBuf;
                    }
                }
            }

            ImGui::Separator();
            std::string jsonPreview = questJsonPreview(*quest);
            ImGui::Text("Preview JSON:");
            ImGui::SameLine();
            if (ImGui::SmallButton("Copy")) {
                ImGui::SetClipboardText(jsonPreview.c_str());
            }
            ImGui::BeginChild("PreviewJson", ImVec2(0, 300), true);
            ImGui::TextWrapped("%s", jsonPreview.c_str());
            ImGui::EndChild();
        }
    } else if (selectedGroupId != -1) { 
        int gIndex = 0;
        QuestGroup* group = findGroupByIndex(rootGroup, gIndex, selectedGroupId);
        if (group) {
            char idBuf[128];
            std::strncpy(idBuf, group->id.c_str(), sizeof(idBuf));
            if (ImGui::InputText("group ID", idBuf, sizeof(idBuf))) group->id = idBuf;

            if (selectedGroupId != 0) {
                char depBuf[256], actBuf[256];
                std::strncpy(depBuf, group->dependencies.c_str(), sizeof(depBuf));
                std::strncpy(actBuf, group->actionAfterComplete.c_str(), sizeof(actBuf));

                if (ImGui::InputText("dependencies", depBuf, sizeof(depBuf))) group->dependencies = depBuf;
                if (ImGui::InputText("actionAfterComplete", actBuf, sizeof(actBuf))) group->actionAfterComplete = actBuf;
            }
            else {
                char nameBuf[256];
                std::strncpy(nameBuf, restaurantName.c_str(), sizeof(nameBuf));
                if (ImGui::InputText("restaurant's name", nameBuf, sizeof(nameBuf))) restaurantName = nameBuf;
                if (ImGui::InputInt("start id", &startId)) { if (startId < 0) startId = 0; }
            }
        }
    }

    ImGui::EndChild();
}


std::string testWidget::questJsonPreview(const Quest& quest) {
    std::stringstream jsonStream;
    jsonStream << "{\n";
    jsonStream << "  \"type\": \"quest\",\n";
    jsonStream << "  \"id\": \"" << quest.id << "\",\n";
    jsonStream << "  \"title\": \"quest." << restaurantName << "." << quest.id << ".title\",\n";
    jsonStream << "  \"cost\": 2,\n";
    if (quest.mainActionType == "decorationAction") {
        jsonStream << "  \"image\": \"images/ftm/interiors/" << restaurantName << "Interior/questManager/" << quest.id << "\",\n";
        jsonStream << "  \"mainAction\": { \"type\": \"" << quest.mainActionType << "\", \"" <<  quest.decorationActionType << "\":";
        if (quest.decorationActionType == "hardSetup") {
            jsonStream <<  "[{\"decorationId\": \"" << quest.affectedDecoration << "\", \"skinId\": null }],\n";
        }
        else if (quest.decorationActionType == "variableSetup") {
            jsonStream << " { \"decorationId\": \"" << quest.affectedDecoration << "\" },\n";
        }
    }
    else if (quest.mainActionType == "customAction") {
        if (quest.controlPoints == true) {
            jsonStream << "  \"image\": \"images/ftm/interiors/" << restaurantName << "Interior/questManager/" << quest.id << "\",\n";
            jsonStream << "  \"mainAction\": { \"type\": \"" << quest.mainActionType << "\", :";
            std::string newLookAction = restaurantName + "_" + quest.id + "_mainAction";
            jsonStream << " \"prepareForActions\": \"true\", \"actionId\": \"" << newLookAction << "\" },\n";
            jsonStream << "        \"controlPoints\": [{\"ownerSkin\": \"after_1\"}]\n";
        }
        else {
            jsonStream << "  \"image\": \"images/ftm/interiors/" << restaurantName << "Interior/questManager/final_show" << "\",\n";
            jsonStream << "  \"mainAction\": { \"type\": \"" << quest.mainActionType << "\", \"" <<  quest.decorationActionType << "\":";
            std::string finalShowAction = restaurantName + quest.id + "mainAction";
            jsonStream << "\"actionId\": \"" << finalShowAction << "\" },\n";
        }
    }
    if (!quest.dependencies.empty()) {
        jsonStream << "  \"dependencies\": [";
        for (size_t i = 0; i < quest.dependencies.size(); ++i) {
            jsonStream << "\"" << quest.dependencies[i] << "\"";
            if (i < quest.dependencies.size() - 1) {
                jsonStream << ", ";
            }
        }
        jsonStream << "],\n";
    }
    jsonStream << "}\n";
    return jsonStream.str();
}

std::string testWidget::generateMainAction(const Quest& quest) {
    return {};
}



QuestGroup* testWidget::findGroupByIndex(QuestGroup& group, int& currentIndex, int targetIndex) {
    if (currentIndex == targetIndex)
        return &group;

    currentIndex++;

    for (auto& sub : group.subGroups) {
        QuestGroup* result = findGroupByIndex(*sub, currentIndex, targetIndex);
        if (result)
            return result;
    }
    return nullptr;
}

Quest* testWidget::findQuestByIndex(QuestGroup& group, int& currentIndex, int targetIndex) {
    for (auto& quest : group.quests) {
        if (currentIndex == targetIndex)
            return &quest;
        currentIndex++;
    }

    for (auto& sub : group.subGroups) {
        Quest* result = findQuestByIndex(*sub, currentIndex, targetIndex);
        if (result)
            return result;
    }

    return nullptr;
}

bool testWidget::removeQuestByIndex(QuestGroup& group, int& currentIndex, int targetIndex) {
    for (size_t i = 0; i < group.quests.size(); ++i) {
        if (currentIndex == targetIndex) {
            freeQuestIndices.push_back(targetIndex);
            group.quests.erase(group.quests.begin() + i);
            return true;
        }
        ++currentIndex;
    }

    for (auto& sub : group.subGroups) {
        if (removeQuestByIndex(*sub, currentIndex, targetIndex))
            return true;
    }
    return false;
}

bool testWidget::removeGroupByIndex(QuestGroup& group, int& currentIndex, int targetIndex) {
    for (auto it = group.subGroups.begin(); it != group.subGroups.end(); ) {
        if (currentIndex == targetIndex - 1) {
            freeGroupIndices.push_back(targetIndex);
            it = group.subGroups.erase(it);
            return true;
        }

        ++currentIndex;

        if (removeGroupByIndex(**it, currentIndex, targetIndex)) {
            return true;
        }

        ++it;
    }
    return false;
}

bool testWidget::isDuplicateGroupId(const QuestGroup& groupToEdit, const std::string& id) {
    std::function<bool(const QuestGroup&)> check = [&](const QuestGroup& group) -> bool {
        if (&group != &groupToEdit && group.id == id)
            return true;
        for (const auto& sub : group.subGroups)
            if (check(*sub)) return true;
        return false;
    };
    return check(rootGroup);
}

bool testWidget::isDuplicateQuestId(const Quest* currentQuest, const std::string& id) {
    std::function<bool(const QuestGroup&)> check = [&](const QuestGroup& group) -> bool {
        for (const auto& q : group.quests) {
            if (&q != currentQuest && q.id == id)
                return true;
        }
        for (const auto& sub : group.subGroups)
            if (check(*sub)) return true;
        return false;
    };
    return check(rootGroup);
}

int testWidget::getNextGroupIndex() {
    if (!freeGroupIndices.empty()) {
        std::sort(freeGroupIndices.begin(), freeGroupIndices.end());
        int index = freeGroupIndices.front();
        freeGroupIndices.erase(freeGroupIndices.begin());
        return index;
    }
    return ++groupIdCounter;
}

int testWidget::getNextQuestIndex() {
    if (!freeQuestIndices.empty()) {
        std::sort(freeQuestIndices.begin(), freeQuestIndices.end());
        int index = freeQuestIndices.front();
        freeQuestIndices.erase(freeQuestIndices.begin());
        return index;
    }
    return questIdCounter++;
}

void testWidget::DrawRestaurantsList() {
    ImGui::Begin("Restaurant Selector");

    // ---------- FTM project path ----------
    ImGui::BeginChild("PathPicker", ImVec2(400, 0), true);
    ImGui::Text("FTM path:");
    ImGui::InputText("##Path", ftmPath, sizeof(ftmPath));
    if (ImGui::Button("Load JSON")) {
        std::filesystem::path p(ftmPath);
        restaurantsListPath = std::filesystem::path(p / "assets/generic/properties/scenes/restaurantsList.json").string();

        if (!std::filesystem::exists(restaurantsListPath)) {
            Logger::Err("File not found: {}", restaurantsListPath);
            Logger::Err("ftm dir {}", ftmPath);
        }
        loadRestaurantsList(restaurantsListPath);
    }

    ImGui::Separator();

    // ---------- ADD REGION ----------
    ImGui::BeginDisabled(!regionsLoaded);
    if (ImGui::Button("Add Region")) {
        RegionInfo newRegion;
        newRegion.regionName = "REGION_" + std::to_string(regions.size() + 1);
        regions.push_back(newRegion);
    }

    // ---------- ADD RESTAURANT ----------
    {
        ImGui::InputText("Restaurant Name", newRestaurantName, sizeof(newRestaurantName));
        if (ImGui::Button("Add Restaurant")) {
            if (strlen(newRestaurantName) == 0 || selectedRegionIndex < 0 || selectedRegionIndex >= (int)regions.size()) {
                ImGui::OpenPopup("AddError");
            } else {
                std::string nameStr = newRestaurantName;
                int regionIndex = selectedRegionIndex;
        
                Defer([this, nameStr, regionIndex]() {
                    int maxStatId = 0;
                    for (const auto& region : regions) {
                        for (const auto& scene : region.scenes) {
                            if (scene.statId > maxStatId) {
                                maxStatId = scene.statId;
                            }
                        }
                    }
        
                    SceneInfo newScene;
                    newScene.name = nameStr;
                    newScene.ownerId = "mary";
                    newScene.statId = maxStatId + 1;
        
                    regions[regionIndex].scenes.push_back(newScene);
        
                    std::memset(newRestaurantName, 0, sizeof(newRestaurantName));
                });
            }
        }
        // ---------- SAVE RESULT ----------
        if (!regions.empty() && !restaurantsListPath.empty()) {
            if (ImGui::Button("Save List")) {
                saveRestaurantsList(restaurantsListPath);
                showSavePopup = true;
                savePopupTimer = 2.0f;
                lastSavePopupPos = ImGui::GetItemRectMin();
            }
        }

        ImGui::Separator();
        {
            if (ImGui::Button("Open Quest Editor")) {
                if (!selectedRestaurant.empty()) {
                    restaurantName = selectedRestaurant;
                    restaurantIsChosenForEditing = true;

                    std::filesystem::path questsPath = std::filesystem::path(ftmPath) / "assets/generic/properties/quests" / (restaurantName + ".json");
                    if (std::filesystem::exists(questsPath)) {
                        loadQuestTreeFromJson(questsPath);
                    } else {
                        Logger::Err("Quest file not found: {}", questsPath.string());
                    }
                } else {
                    ImGui::OpenPopup("SelectRegion");
                }
            }
        }

        Defer([this]() {
            if (showSavePopup) {
                savePopupTimer -= ImGui::GetIO().DeltaTime;
                if (savePopupTimer <= 0.0f) {
                    showSavePopup = false;
                } else {
                    ImVec2 popupSize(120, 40);
                    ImVec2 popupOffset = {0, 30};
                    ImVec2 pos = ImVec2(lastSavePopupPos.x + popupOffset.x,
                                        lastSavePopupPos.y + popupOffset.y);
                    ImGui::SetNextWindowPos(pos);
                    ImGui::SetNextWindowBgAlpha(0.8f);

                    ImGui::Begin("##SavedPopup", nullptr,
                        ImGuiWindowFlags_NoDecoration |
                        ImGuiWindowFlags_AlwaysAutoResize |
                        ImGuiWindowFlags_NoFocusOnAppearing |
                        ImGuiWindowFlags_NoNav |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoInputs
                    );
                    ImGui::Text("Saved!");
                    ImGui::End();
                }
            }
        });
    }
    ImGui::EndDisabled();

    ImGui::EndChild();
    ImGui::SameLine();

    // ----------------- Restaurant list ----------------
    ImGui::BeginChild("RestaurantList", ImVec2(0, 0), true);
    if (!regionsLoaded && regions.empty()) {
        ImGui::Text("Load JSON or add region.");
    } else {
        for (size_t i = 0; i < regions.size(); ++i) {
            auto& region = regions[i];
            ImGuiTreeNodeFlags flags = (selectedRegionIndex == (int)i) ? ImGuiTreeNodeFlags_Selected : 0;

            bool open = ImGui::TreeNodeEx(region.regionName.c_str(), flags);
            if (ImGui::IsItemClicked()) {
                selectedRegionIndex = static_cast<int>(i);
            }

            if (open) {
                for (auto& scene : region.scenes) {
                    bool selected = (selectedRestaurant == scene.name);
                    if (ImGui::Selectable(scene.name.c_str(), selected)) {
                        selectedRestaurant = scene.name;
                    }
                }
                ImGui::TreePop();
            }
        }
    }
    ImGui::EndChild();
    ImGui::End();
}

void testWidget::loadRestaurantsList(const std::filesystem::path& jsonPath) {
    regions.clear();
    regionsLoaded = false;

    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        Logger::Err("Failed to open JSON file: {}", jsonPath.string());
        return;
    }

    nlohmann::json j;
    file >> j;
    if (j.is_null()) {
        Logger::Err("Failed to parse JSON file.");
        return;
    }

    for (const auto& regionJson : j) {
        RegionInfo region;
        region.regionName = regionJson.value("name", "unknown_region");
        region.statId = regionJson.value("stat_id", 0);
        region.itemId = regionJson.value("item_id", "");

        for (const auto& sceneJson : regionJson["scenes"]) {
            SceneInfo scene;
            scene.name = sceneJson.value("name", "unknown");
            scene.ownerId = sceneJson.value("owner_id", "");
            scene.statId = sceneJson.value("stat_id", 0);
            scene.parent_scene_id = sceneJson.value("parent_scene_id", "");
            region.scenes.push_back(scene);
        }
        regions.push_back(region);
    }
    regionsLoaded = true;
}

void testWidget::saveRestaurantsList(const std::string& fullPath) {
    nlohmann::json j = nlohmann::json::array();

    for (const auto& region : regions) {
        nlohmann::json regionJson;
        regionJson["item_id"] = region.itemId;
        regionJson["name"] = region.regionName;
        regionJson["stat_id"] = region.statId;

        for (const auto& scene : region.scenes) {
            nlohmann::json sceneJson;
            sceneJson["name"] = scene.name;
            sceneJson["owner_id"] = scene.ownerId;
            sceneJson["stat_id"] = scene.statId;
            if (!scene.parent_scene_id.empty()) {
                sceneJson["parent_scene_id"] = scene.parent_scene_id;
            }
            regionJson["scenes"].push_back(sceneJson);
        }

        j.push_back(regionJson);

    }

    std::ofstream out(fullPath);
    if (out.is_open()) {
        out << j.dump(1, '\t');
    }
}

void testWidget::loadQuestTreeFromJson(const std::filesystem::path& jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        Logger::Err("Failed to open quest file: {}", jsonPath.string());
        return;
    }

    nlohmann::json j;
    file >> j;
    if (!j.contains("mainGroup") || !j["mainGroup"].is_object()) {
        Logger::Err("Invalid quest file format");
        return;
    }

        rootGroup = parseGroupFromJson(j["mainGroup"]);

    int extractedId = 0;
    std::function<void(const QuestGroup&)> extract = [&](const QuestGroup& group) {
        for (const auto& q : group.quests) {
            try {
                extractedId = std::stoi(q.id);
                return;
            } catch (...) {}
        }
        for (const auto& sub : group.subGroups) extract(*sub);
    };
    extract(rootGroup);
    if (extractedId > 0) startId = extractedId;

    rootGroup = parseGroupFromJson(j["mainGroup"]);
}

QuestGroup testWidget::parseGroupFromJson(const nlohmann::json& jgroup) {
    QuestGroup group;
    group.id = jgroup.value("id", "");
    if (jgroup.contains("dependencies")) {
        if (jgroup["dependencies"].is_string())
            group.dependencies = jgroup["dependencies"];
        else if (jgroup["dependencies"].is_array() && !jgroup["dependencies"].empty())
            group.dependencies = jgroup["dependencies"].at(0); // simplified assumption
    }
    if (jgroup.contains("actionAfterComplete")) {
        group.actionAfterComplete = jgroup["actionAfterComplete"].dump();
    }

    for (const auto& child : jgroup["childs"]) {
        if (!child.contains("type")) continue;
        std::string type = child["type"];
        if (type == "quest") {
            Quest quest;
            quest.id = child.value("id", "");
            quest.dependencies = child.value("dependencies", std::vector<std::string>{});
            if (child.contains("mainAction")) {
                auto& action = child["mainAction"];
                quest.mainActionType = action.value("type", "");
                if (quest.mainActionType == "decorationAction") {
                    if (action.contains("hardSetup")) {
                        quest.decorationActionType = "hardSetup";
                        auto& setup = action["hardSetup"][0];
                        quest.affectedDecoration = setup.value("decorationId", "");
                        quest.decorationSkin = setup.contains("skinId") && !setup["skinId"].is_null() ? setup["skinId"].get<std::string>() : "";
                    } else if (action.contains("variableSetup")) {
                        quest.decorationActionType = "variableSetup";
                        quest.affectedDecoration = action["variableSetup"].value("decorationId", "");
                    }
                } else if (quest.mainActionType == "customAction") {
                    quest.controlPoints = child.contains("controlPoints");
                }
            }
            group.quests.push_back(std::move(quest));
        } else if (type == "group") {
            group.subGroups.push_back(std::make_unique<QuestGroup>(parseGroupFromJson(child)));
        }
    }

    return group;
}

void testWidget::saveQuestTreeToJson(const std::filesystem::path& jsonPath) {
    nlohmann::json result;
    result["mainGroup"] = serializeGroupToJson(rootGroup);

    std::ofstream out(jsonPath);
    if (!out.is_open()) {
        Logger::Err("Failed to write quest file: {}", jsonPath.string());
        return;
    }
    out << result.dump(1, '\t');
}

nlohmann::json testWidget::serializeGroupToJson(const QuestGroup& group) {
    nlohmann::json jgroup;
    jgroup["type"] = "group";
    jgroup["id"] = group.id;
    if (!group.dependencies.empty()) jgroup["dependencies"] = { group.dependencies };
    if (!group.actionAfterComplete.empty()) jgroup["actionAfterComplete"] = nlohmann::json::parse(group.actionAfterComplete);

    for (const auto& quest : group.quests) {
        nlohmann::json jquest;
        jquest["type"] = "quest";
        jquest["id"] = quest.id;
        jquest["title"] = "quest." + restaurantName + "." + quest.id + ".title";
        jquest["cost"] = 2; // default
        jquest["image"] = "images/ftm/interiors/" + restaurantName + "Interior/questManager/" + quest.id;
        jquest["mainAction"] = { {"type", quest.mainActionType} };

        if (quest.mainActionType == "decorationAction") {
            if (quest.decorationActionType == "hardSetup") {
                jquest["mainAction"]["hardSetup"] = {{ {"decorationId", quest.affectedDecoration}, {"skinId", quest.decorationSkin.empty() ? nullptr : nlohmann::json(quest.decorationSkin)} }};
            } else if (quest.decorationActionType == "variableSetup") {
                jquest["mainAction"]["variableSetup"] = { {"decorationId", quest.affectedDecoration} };
            }
        } else if (quest.mainActionType == "customAction") {
            jquest["mainAction"]["actionId"] = restaurantName + "_" + quest.id + "_mainAction";
            if (quest.controlPoints) jquest["controlPoints"] = {{ {"ownerSkin", "after_1"} }};
        }

        if (!quest.dependencies.empty()) {
            jquest["dependencies"] = quest.dependencies;
        }
        jgroup["childs"].push_back(jquest);
    }

    for (const auto& sub : group.subGroups) {
        jgroup["childs"].push_back(serializeGroupToJson(*sub));
    }

    return jgroup;
}


void testWidget::Defer(std::function<void()> fn) {
    deferredCallbacks.push_back(std::move(fn));
}


} 