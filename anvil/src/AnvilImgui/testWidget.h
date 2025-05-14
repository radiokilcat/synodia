#pragma once

#include "ImguiSystem.h"
#include <filesystem>
#include "imgui.h"
#include "nlohmann/json.hpp"

namespace anvil {
	
	class IWidget;

	struct SceneInfo {
		std::string name;
		std::string ownerId;
		int statId;
		std::string parent_scene_id;
	};
	
	struct RegionInfo {
		std::string regionName;
		std::string itemId;
		int statId;
		std::vector<SceneInfo> scenes;
	};

	struct Quest {
		std::string id;
		std::string mainActionType;
		std::string decorationActionType;
		std::string affectedDecoration;
		std::string decorationSkin;
		std::vector<std::string> dependencies;
		bool controlPoints;
	};
	
	struct QuestGroup {
		std::string id;
		std::string dependencies;
		std::string actionAfterComplete;
		std::vector<Quest> quests;
		std::vector<std::unique_ptr<QuestGroup>> subGroups;
	};
	
	class testWidget : public IWidget {
	public:
		testWidget();
		void draw() override;
        void DrawQuestEditorWidget();
        void drawTreeButtons();
        void drawGroupTree(QuestGroup &group, int &groupCounter, int &questCounter);
        void drawRightPanel();

        std::string questJsonPreview(const Quest &quest);
        std::string generateMainAction(const Quest &quest);

        QuestGroup *findGroupByIndex(QuestGroup &group, int &currentIndex, int targetIndex);
        Quest* findQuestByIndex(QuestGroup& group, int& currentIndex, int targetIndex);

		bool removeQuestByIndex(QuestGroup& group, int& currentIndex, int targetIndex);
		bool removeGroupByIndex(QuestGroup& group, int& currentIndex, int targetIndex);

		bool isDuplicateGroupId(const QuestGroup& groupToEdit, const std::string& id);
        bool isDuplicateQuestId(const Quest *currentQuest, const std::string &id);

        void loadRestaurantsList(const std::filesystem::path& jsonPath);
        void loadQuestTreeFromJson(const std::filesystem::path& jsonPath);

        void saveQuestTreeToJson(const std::filesystem::path &jsonPath);
        void saveRestaurantsList(const std::string &fullPath);

        int getNextGroupIndex();
        int getNextQuestIndex();

        void DrawRestaurantsList();


    private:
        QuestGroup parseGroupFromJson(const nlohmann::json &jgroup);
        nlohmann::json serializeGroupToJson(const QuestGroup &group);
        void Defer(std::function<void()> fn);

        QuestGroup rootGroup;

		std::vector<int> freeGroupIndices;
		std::vector<int> freeQuestIndices;

		int selectedGroupId = -1;
		int selectedQuestId = -1;
		int groupIdCounter = 0;
		int questIdCounter = 0;

		std::string restaurantName;
		int startId = 0;
		bool restaurantIsChosenForEditing = false;

		std::vector<RegionInfo> regions;
		std::string selectedRestaurant;
		std::string restaurantsListPath;
		int selectedRegionIndex = -1;
		char newRestaurantName[128] = ""; 
		char ftmPath[512] = "";
		bool regionsLoaded = false;
		std::vector<std::function<void()>> deferredCallbacks;
		bool showSavePopup = false;
		float savePopupTimer = 0.0f;
		ImVec2 lastSavePopupPos = ImVec2(0.0f, 0.0f);
    };
}
