#pragma once

#include "AnvilImgui/ImguiSystem.h"

void ImguiSystem::init(std::unique_ptr<anvil::Window>& window, SDL_Renderer* renderer) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io{ImGui::GetIO()};

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui_ImplSDL3_InitForSDLRenderer(window->getWindow(), renderer );
    ImGui_ImplSDLRenderer3_Init( renderer );
}

void ImguiSystem::drawWidgets() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowPos(ImVec2(200,200), ImGuiCond_FirstUseEver);
    ImGui::Begin("My First Tool", NULL, ImGuiWindowFlags_MenuBar );
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) {  }
            if (ImGui::MenuItem("Save", "Ctrl+S"))   {  }
            if (ImGui::MenuItem("Close", "Ctrl+W"))  {  }
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
    ImGui::TextColored(ImVec4(1,1,0,1), "Important Stuff");
    ImGui::BeginChild("Scrolling");
    for (int n = 0; n < 50; n++)
        ImGui::Text("%04d: Some text", n);
    ImGui::EndChild();
    ImGui::End();
    ImGui::Render();
}

static ImguiSystem* m_instance = nullptr;

ImguiSystem* ImguiSystem::Instance() {
    if(m_instance == nullptr)
    {
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
    ImGui_ImplSDLRenderer3_RenderDrawData( ImGui::GetDrawData() );
}

void ImguiSystem::shutDown() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void ImguiSystem::drawMenuBar() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..", "Ctrl+O")) {  }
            if (ImGui::MenuItem("Save", "Ctrl+S"))   {  }
            if (ImGui::MenuItem("Close", "Ctrl+W"))  {  }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
    ImGui::Render();
}

