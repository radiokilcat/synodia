#include "AnvilImgui/ImguiSystem.h"
#include "application.h"
#include "states/editstate.h"

namespace anvil {

    void ImguiSystem::init(std::unique_ptr<anvil::Window> &window, SDL_Renderer *renderer) {
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
                    if (ImGui::MenuItem("Close", "Ctrl+W")) {}
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
            if (showInspector) {
                ImGui::Begin("test", NULL, ImGuiWindowFlags_None);
                if (ImGui::BeginTable("##split", 2, ImGuiTableFlags_BordersOuter | ImGuiTableFlags_Resizable |
                                                    ImGuiTableFlags_ScrollY)) {
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableSetupColumn("Object");
                    ImGui::TableSetupColumn("Contents");
                    ImGui::TableHeadersRow();

                    drawNodes(rootNode);

                    ImGui::EndTable();
                }
                ImGui::End();
            }
        });
    }

    void ImguiSystem::addGameObject(anvil::GameObject* obj) {
        objects.push_back(obj);
    }

    void ImguiSystem::setRootNode(anvil::GameObject* node) {
        rootNode = node;
    }

    void ImguiSystem::drawSingleNode(anvil::GameObject *node) {
        if (node == nullptr) return;
        ImGui::PushID(node->getId().c_str());
        ImGui::TableNextRow();
        ImGui::TableSetColumnIndex(0);
        ImGui::AlignTextToFramePadding();
        bool node_open = ImGui::TreeNode("Object", "%s", node->getId().c_str());
        ImGui::TableSetColumnIndex(1);

        if (node_open)
        {
            static float placeholder_members[8] = { 0.0f, 0.0f, 1.0f, 3.1416f, 100.0f, 999.0f };
            for (int i = 0; i < 8; i++)
            {
                ImGui::PushID(i); // Use field index as identifier.
                if (i < 2)
                {
//                    ShowPlaceholderObject("Child", 424242);
                }
                else
                {
                    // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
                    ImGui::TableNextRow();
                    ImGui::TableSetColumnIndex(0);
                    ImGui::AlignTextToFramePadding();
                    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet;
                    ImGui::TreeNodeEx("Field", flags, "Field_%d", i);

                    ImGui::TableSetColumnIndex(1);
                    ImGui::SetNextItemWidth(-FLT_MIN);
                    if (i >= 5)
                        ImGui::InputFloat("##value", &placeholder_members[i], 1.0f);
                    else
                        ImGui::DragFloat("##value", &placeholder_members[i], 0.01f);
                    ImGui::NextColumn();
                }
                ImGui::PopID();
            }
            ImGui::TreePop();
        }
        ImGui::PopID();
    }
void ImguiSystem::drawNodes(anvil::GameObject* node) {
        if (node == nullptr) return;
        drawSingleNode(node);

        for (const auto &child: node->getChildren()) {
            auto obj = dynamic_cast<anvil::GameObject*>(child.get());
            drawSingleNode(obj);

        }
    }

}