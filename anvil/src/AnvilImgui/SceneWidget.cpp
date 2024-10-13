#include "AnvilImgui/SceneWidget.h"

#include <iostream>

#include "imgui.h"
#include "components/Sprite2DComponent.h"
#include "components/Transform2DComponent.h"

namespace anvil {
	
void GameSceneWidget::draw() {
    ImGui::Begin("test", NULL, ImGuiWindowFlags_None);
    if (ImGui::BeginTable("##split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable |
                                        ImGuiTableFlags_ScrollY)) {
        ImGui::TableSetupScrollFreeze(0, 1);
        ImGui::TableSetupColumn("Object");
        ImGui::TableSetupColumn("Contents");
        ImGui::TableHeadersRow();
        drawNode(scene);
        ImGui::EndTable();
    }
    ImGui::End();
}
	
void GameSceneWidget::drawWidgets() {
    ImGui::SetNextWindowPos(ImVec2(200, 200), ImGuiCond_FirstUseEver);
    ImGui::Begin("My First Tool", NULL, ImGuiWindowFlags_MenuBar);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Close", "Ctrl+W")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
    float my_color[] = {0, 0, 255.f};

    // Edit a color stored as 4 floats
    ImGui::ColorEdit4("Color", my_color);

    // Generate samples and plot them
    float samples[100];
    for (int n = 0; n < 100; n++)
        samples[n] = sinf(n * 0.2f + ImGui::GetTime() * 1.5f);
    ImGui::PlotLines("Samples", samples, 100);

    // Display contents in a scrolling region
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++)
        ImGui::Text("%04d: Some text", n);
    ImGui::EndChild();
    ImGui::End();
}
    
void GameSceneWidget::addGameObject(GameObject* obj) {
    objects.push_back(obj);
}

void GameSceneWidget::setRootNode(std::shared_ptr<GameObject> node) {
    scene = node;
}

void GameSceneWidget::drawPropertiesWidget(std::shared_ptr<GameObject> node) {
    ImGui::SetCursorPosY(ImGui::GetTextLineHeight() * 2);
    if (!node.get())
        return;
    // Transform
    {
        ImGui::SeparatorText("TransformComponent");
        auto transform = node->getComponent<Transform2DComponent>();
        if (!transform)
            return;
        
        float x, y = 0.f;
        x = transform->getX();
        y = transform->getY();
        ImGui::PushItemWidth(200);
        if (ImGui::InputFloat("x    ", &x, 10.f, 10.f)) {
            transform->setX(x);
        }
        ImGui::SameLine();
        if (ImGui::InputFloat("y", &y, 10.f, 10.f)) {
            transform->setY(y);
        }
        float width, height = 0.f;
        width = transform->getWidth();
        height = transform->getHeight();
        if (ImGui::InputFloat("width", &width, 10.f, 10.f)) {
            transform->setWidth(width);
        }
        ImGui::SameLine();
        if (ImGui::InputFloat("height", &height, 10.f, 10.f)) {
            transform->setHeight(height);
        }
    }
    //Sprite
    {
        ImGui::SeparatorText("Sprite");
        auto sprite = node->getComponent<Sprite2DComponent>();
        if (!sprite)
            return;
        float width, height = 0.f;
        width = sprite->width();
        height = sprite->height();
        if (ImGui::InputFloat("width", &width, 10.f, 10.f)) {
            sprite->setWidth(width);
        }
        ImGui::SameLine();
        if (ImGui::InputFloat("height", &height, 10.f, 10.f)) {
            sprite->setHeight(height);
        }
        int row = sprite->getCurrentRow();
        int frame = sprite->getCurrentFrame();
        if (ImGui::InputInt("currentrow", &row, 1)) {
            sprite->setCurrentRow(row);
        }
        if (ImGui::InputInt("currentframe", &frame, 1)) {
            sprite->setCurrentFrame(row);
        }
        //

       constexpr size_t bufferSize = 256;
       char buffer[bufferSize];
       std::string str0 = sprite->getTextureId();
       std::strncpy(buffer, str0.c_str(), bufferSize);
       buffer[bufferSize - 1] = '\0';
       if (ImGui::InputText("current sheet", buffer, bufferSize)) {
           sprite->setCurrentSheet(std::string(buffer)); 
       }
    }
}

void GameSceneWidget::drawSingleNode(std::shared_ptr<GameObject> node) {
    ImGui::PushID(node->getId().c_str());
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::AlignTextToFramePadding();
    bool node_open = ImGui::TreeNode("Object", "%s", node->getId().c_str());

    if (node_open)
    {
        auto children = node->getChildren();
        if ( children.size() == 0 )
        {
            ImGui::Text(node->getId().c_str());
            ImGui::NextColumn();

            ImGui::Button(node->getId().c_str());
            ImGui::NextColumn();
        }
        else {
            for (auto i: children) {
                auto obj = std::dynamic_pointer_cast<GameObject>(i);
                ImGui::PushID(obj->getId().c_str()); // Use field index as identifier.
                ImGui::TableNextRow();
                ImGui::TableSetColumnIndex(0);
                ImGui::AlignTextToFramePadding();
                ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen ;
                if (ImGui::TreeNodeEx(obj->getId().c_str(), flags)) {
                    if (ImGui::IsItemClicked()) {
                        currentObj = obj;
                    }
                }
                ImGui::PopID();
            }
        }
        ImGui::TableSetColumnIndex(1);
        drawPropertiesWidget(currentObj);
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void GameSceneWidget::drawNode(std::weak_ptr<GameObject> node) {
    if (auto root = node.lock()) {
        drawSingleNode(root);
    }
    else {
        std::cout << "cannot lock the object" << std::endl;
    }
}
}
