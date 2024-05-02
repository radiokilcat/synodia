#include "AnvilImgui/ImguiSystem.h"
#include "application.h"
#include "../../../apps/test_app/gameobjects/gamescene.h"
#include "components/Sprite2DComponent.h"
#include "states/editstate.h"

namespace anvil {

    void ImguiSystem::init(std::unique_ptr<Window> &window, SDL_Renderer *renderer) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io{ImGui::GetIO()};

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        ImGui_ImplSDL3_InitForSDLRenderer(window->getWindow(), renderer);
        ImGui_ImplSDLRenderer3_Init(renderer);
    }

    void ImguiSystem::drawWidgets() {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
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
        ImGui::Render();
    }

    static ImguiSystem *m_instance = nullptr;

    ImguiSystem *ImguiSystem::Instance() {
        if (m_instance == nullptr) {
            m_instance = new ImguiSystem();
            return m_instance;
        }
        return m_instance;
    }

    ImguiSystem::ImguiSystem() {

    }

    ImguiSystem::~ImguiSystem() {
    }

    void ImguiSystem::render() {
        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();
        while (!callbacks.empty()) {
            callbacks.front()();
            callbacks.pop();
        }
        ImGui::Render();
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImguiSystem::shutDown() {
        ImGui_ImplSDLRenderer3_Shutdown();
        ImGui_ImplSDL3_Shutdown();
        ImGui::DestroyContext();
    }

    void ImguiSystem::drawMenuBar() {
        callbacks.push([this]() {
            if (ImGui::BeginMainMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("Open..", "Ctrl+O")) {}
                    if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                    if (ImGui::MenuItem("Imgui Demo", "Ctrl+W")) {
                        show_demo_window = !show_demo_window;
                    }
                    if (ImGui::MenuItem("Editor", "Ctrl+W")) {
                        Application::Instance()->getStateMachine()->changeState(new EditState);
                    }
                    if (ImGui::MenuItem("debug view")) {
                        showInspector = !showInspector;
                    }
                    ImGui::EndMenu();
                }
                ImGui::EndMainMenuBar();
            }
            if (show_demo_window) {
                ImGui::ShowDemoWindow(&show_demo_window);
            }
            if (showInspector) {
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
        });
    }

    void ImguiSystem::addGameObject(GameObject* obj) {
        objects.push_back(obj);
    }

    void ImguiSystem::setRootNode(std::shared_ptr<GameObject> node) {
        scene = node;
    }

    void ImguiSystem::drawPropertiesWidget(std::shared_ptr<GameObject> node) {
        ImGui::SetCursorPosY(ImGui::GetTextLineHeight() * 2);
        if (!node.get())
            return;
        // Transform
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
        // 
        //Sprite
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
        ImGui::SeparatorText("Sprite");
        int row = sprite->getCurrentRow();
        int frame = sprite->getCurrentFrame();
        if (ImGui::InputInt("currentrow", &row, 1)) {
            sprite->setCurrentRow(row);
        }
        if (ImGui::InputInt("currentframe", &frame, 1)) {
            sprite->setCurrentFrame(row);
        }
        //
        ImGui::SeparatorText("Sprite");

       constexpr size_t bufferSize = 256;
       char buffer[bufferSize];
       std::string str0 = sprite->getTextureId();
       std::strncpy(buffer, str0.c_str(), bufferSize);
       buffer[bufferSize - 1] = '\0';
       if (ImGui::InputText("current sheet", buffer, bufferSize)) {
           sprite->setCurrentSheet(std::string(buffer)); 
       }
    }
    
    void ImguiSystem::drawSingleNode(std::shared_ptr<GameObject> node) {
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

    void ImguiSystem::drawNode(std::weak_ptr<GameObject> node) {
        if (auto root = node.lock()) {
            drawSingleNode(root);
        }
        else {
            std::cout << "cannot lock the object" << std::endl;
        }
    }
}