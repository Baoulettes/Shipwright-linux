#include "SohImGuiImpl.h"

#include <iostream>
#include <utility>
#include "Archive.h"
#include "Environment.h"
#include "GameSettings.h"
#include "SohConsole.h"
#include "SohHooks.h"
#include "Lib/ImGui/imgui_internal.h"
#include "GlobalCtx2.h"
#include "ResourceMgr.h"
#include "TextureMod.h"
#include "Window.h"
#include "Cvar.h"
#include "../Fast3D/gfx_pc.h"
#include "Lib/stb/stb_image.h"
#include "Lib/Fast3D/gfx_rendering_api.h"
#include "Utils/StringHelper.h"
//#include "../../soh/soh/Enhancements/debugconsole.h"

#ifdef ENABLE_OPENGL
#include "Lib/ImGui/backends/imgui_impl_opengl3.h"
#include "Lib/ImGui/backends/imgui_impl_sdl.h"

#endif

#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
#include "Lib/ImGui/backends/imgui_impl_dx11.h"
#include "Lib/ImGui/backends/imgui_impl_win32.h"
#include <Windows.h>

IMGUI_IMPL_API LRESULT  ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

#endif

using namespace Ship;
bool oldCursorState = true;

#define TOGGLE_BTN ImGuiKey_F1
#define HOOK(b) if(b) needs_save = true;
OSContPad* pads;

std::map<std::string, GameAsset*> DefaultAssets;
namespace SohImGui {
    WindowImpl impl;
    ImGuiIO* io;
    Console* console = new Console;
    bool p_open = false;
    bool needs_save = false;

    int SelectedLanguage = CVar_GetS32("gLanguages", 0); //Default Language to English
    int SelectedHUD = CVar_GetS32("gHudColors", 1);      //Default colors to Gamecube.
    float hearts_colors[3] = {0,0,0};
    float hearts_dd_colors[3] = {0,0,0};
    float a_btn_colors[3] = {0,0,0};
    float b_btn_colors[3] = {0,0,0};
    float c_btn_colors[3] = {0,0,0};
    float start_btn_colors[3] = {0,0,0};
    float magic_border_colors[3] = {0,0,0};
    float magic_remaining_colors[3] = {0,0,0};
    float magic_use_colors[3] = {0,0,0};
    float minimap_colors[3] = {0,0,0};
    float rupee_colors[3] = {0,0,0};
    float kokiri_col[3] = { 0.118f, 0.41f, 0.106f };
    float goron_col[3] = { 0.392f, 0.078f, 0.0f };
    float zora_col[3] = { 0.0f, 0.235f, 0.392f };
    float navi_idle_i_col[3] = { 0.0f, 0.0f, 0.0f };
    float navi_idle_o_col[3] = { 0.0f, 0.0f, 0.0f };
    float navi_npc_i_col[3] = { 0.0f, 0.0f, 0.0f };
    float navi_npc_o_col[3] = { 0.0f, 0.0f, 0.0f };
    float navi_enemy_i_col[3] = { 0.0f, 0.0f, 0.0f };
    float navi_enemy_o_col[3] = { 0.0f, 0.0f, 0.0f };
    float navi_prop_i_col[3] = { 0.0f, 0.0f, 0.0f };
    float navi_prop_o_col[3] = { 0.0f, 0.0f, 0.0f };

    void ImGuiWMInit() {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplSDL2_InitForOpenGL(static_cast<SDL_Window*>(impl.sdl.window), impl.sdl.context);
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplWin32_Init(impl.dx11.window);
            break;
#endif
        default:
            break;
        }
        
    }

    void ImGuiBackendInit() {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplOpenGL3_Init("#version 120");
            break;

#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplDX11_Init(static_cast<ID3D11Device*>(impl.dx11.device), static_cast<ID3D11DeviceContext*>(impl.dx11.device_context));
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiProcessEvent(EventImpl event) {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplSDL2_ProcessEvent(static_cast<const SDL_Event*>(event.sdl.event));
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplWin32_WndProcHandler(static_cast<HWND>(event.win32.handle), event.win32.msg, event.win32.wparam, event.win32.lparam);
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiWMNewFrame() {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplSDL2_NewFrame(static_cast<SDL_Window*>(impl.sdl.window));
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplWin32_NewFrame();
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiBackendNewFrame() {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplOpenGL3_NewFrame();
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplDX11_NewFrame();
            break;
#endif
        default:
            break;
        }
    }

    void ImGuiRenderDrawData(ImDrawData* data) {
        switch (impl.backend) {
        case Backend::SDL:
            ImGui_ImplOpenGL3_RenderDrawData(data);
            break;
#if defined(ENABLE_DX11) || defined(ENABLE_DX12)
        case Backend::DX11:
            ImGui_ImplDX11_RenderDrawData(data);
            break;
#endif
        default:
            break;
        }
    }

    void LoadCVars() {
        FILE* fp; //Cvars file.
        char* line = NULL;
        size_t len = 0;
        ssize_t read;
        fp = fopen("cvars.cfg", "r");
        if(fp != NULL) {
            while ((read = getline(&line, &len, fp)) != -1) {
                SohImGui::console->Dispatch(line);
                printf("Loaded : %s",line);
            }
        }
    }

	void SaveCVars() {
		std::string output;
		std::ofstream cvarfile;
		cvarfile.open("cvars.cfg");
		for (const auto &cvar : cvars) {
		    if (cvar.second->type == CVAR_TYPE_STRING)
		        output += StringHelper::Sprintf("set %s %s\n", cvar.first.c_str(), cvar.second->value.valueStr);
		    else if (cvar.second->type == CVAR_TYPE_S32)
		        output += StringHelper::Sprintf("set %s %i\n", cvar.first.c_str(), cvar.second->value.valueS32);
		    else if (cvar.second->type == CVAR_TYPE_FLOAT)
		        output += StringHelper::Sprintf("set %s %f\n", cvar.first.c_str(), cvar.second->value.valueFloat);
		}
		cvarfile << output.c_str();
		cvarfile.close();
	}
	
    bool UseViewports() {
        switch (impl.backend) {
        case Backend::DX11:
            return true;
        default:
            return false;
        }
    }

    void ShowCursor(bool hide, Dialogues d) {
        if (d == Dialogues::dLoadSettings) {
            GlobalCtx2::GetInstance()->GetWindow()->ShowCursor(hide);
            return;
        }

        if (d == Dialogues::dConsole && Game::Settings.debug.menu_bar) {
            return;
        }
        if (!GlobalCtx2::GetInstance()->GetWindow()->IsFullscreen()) {
            oldCursorState = false;
            return;
        }

        if (oldCursorState != hide) {
            oldCursorState = hide;
            GlobalCtx2::GetInstance()->GetWindow()->ShowCursor(hide);
        }
    }

    void LoadTexture(std::string name, std::string path) {
        GfxRenderingAPI* api = gfx_get_current_rendering_api();
        const auto res = GlobalCtx2::GetInstance()->GetResourceManager()->LoadFile(normalize(path));

        const auto asset = new GameAsset{ api->new_texture() };
        uint8_t* img_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(res->buffer.get()), res->dwBufferSize, &asset->width, &asset->height, nullptr, 4);

        if (img_data == nullptr) {
            std::cout << "Found error: " << stbi_failure_reason() << std::endl;
            return;
        }

        api->select_texture(0, asset->textureId);
        api->set_sampler_parameters(0, false, 0, 0);
        api->upload_texture(img_data, asset->width, asset->height);

        DefaultAssets[name] = asset;
        stbi_image_free(img_data);
    }

    void LoadCosmeticColors(){//This function is necessary as without it IMGui wont load the updated float array.
        hearts_colors[0] = (float)CVar_GetS32("gCCHeartsPrimR", 255)/255;
        hearts_colors[1] = (float)CVar_GetS32("gCCHeartsPrimG", 10)/255;
        hearts_colors[2] = (float)CVar_GetS32("gCCHeartsPrimB", 10)/255;
        hearts_dd_colors[0] = (float)CVar_GetS32("gDDCCHeartsPrimR", 255)/255;
        hearts_dd_colors[1] = (float)CVar_GetS32("gDDCCHeartsPrimG", 255)/255;
        hearts_dd_colors[2] = (float)CVar_GetS32("gDDCCHeartsPrimB", 255)/255;
        a_btn_colors[0] = (float)CVar_GetS32("gCCABtnPrimR", 90)/255;
        a_btn_colors[1] = (float)CVar_GetS32("gCCABtnPrimG", 90)/255;
        a_btn_colors[2] = (float)CVar_GetS32("gCCABtnPrimB", 255)/255;
        b_btn_colors[0] = (float)CVar_GetS32("gCCBBtnPrimR", 0)/255;
        b_btn_colors[1] = (float)CVar_GetS32("gCCBBtnPrimG", 150)/255;
        b_btn_colors[2] = (float)CVar_GetS32("gCCBBtnPrimB", 0)/255;
        c_btn_colors[0] = (float)CVar_GetS32("gCCCBtnPrimR", 255)/255;
        c_btn_colors[1] = (float)CVar_GetS32("gCCCBtnPrimG", 160)/255;
        c_btn_colors[2] = (float)CVar_GetS32("gCCCBtnPrimB", 0)/255;
        start_btn_colors[0] = (float)CVar_GetS32("gCCStartBtnPrimR", 120)/255;
        start_btn_colors[1] = (float)CVar_GetS32("gCCStartBtnPrimG", 120)/255;
        start_btn_colors[2] = (float)CVar_GetS32("gCCStartBtnPrimB", 120)/255;
        magic_border_colors[0] = (float)CVar_GetS32("gCCMagicBorderPrimR", 255)/255;
        magic_border_colors[1] = (float)CVar_GetS32("gCCMagicBorderPrimG", 255)/255;
        magic_border_colors[2] = (float)CVar_GetS32("gCCMagicBorderPrimB", 255)/255;
        magic_remaining_colors[0] = (float)CVar_GetS32("gCCMagicPrimR", 250)/255;
        magic_remaining_colors[1] = (float)CVar_GetS32("gCCMagicPrimG", 250)/255;
        magic_remaining_colors[2] = (float)CVar_GetS32("gCCMagicPrimB", 0)/255;
        magic_remaining_colors[0] = (float)CVar_GetS32("gCCMagicUsePrimR", 0)/255;
        magic_remaining_colors[1] = (float)CVar_GetS32("gCCMagicUsePrimG", 200)/255;
        magic_remaining_colors[2] = (float)CVar_GetS32("gCCMagicUsePrimB", 0)/255;
        minimap_colors[0] = (float)CVar_GetS32("gCCMinimapPrimR", 0)/255;
        minimap_colors[1] = (float)CVar_GetS32("gCCMinimapPrimG", 255)/255;
        minimap_colors[2] = (float)CVar_GetS32("gCCMinimapPrimB", 255)/255;
        rupee_colors[0] = (float)CVar_GetS32("gCCRupeePrimR", 120)/255;
        rupee_colors[1] = (float)CVar_GetS32("gCCRupeePrimG", 120)/255;
        rupee_colors[2] = (float)CVar_GetS32("gCCRupeePrimB", 120)/255;
        kokiri_col[0] = (float)CVar_GetS32("gTunic_Kokiri_R", 30)/255;
        kokiri_col[1] = (float)CVar_GetS32("gTunic_Kokiri_G", 105)/255;
        kokiri_col[2] = (float)CVar_GetS32("gTunic_Kokiri_B", 27)/255;
        goron_col[0] = (float)CVar_GetS32("gTunic_Goron_R", 100)/255;
        goron_col[1] = (float)CVar_GetS32("gTunic_Goron_G", 20)/255;
        goron_col[2] = (float)CVar_GetS32("gTunic_Goron_B", 0)/255;
        zora_col[0] = (float)CVar_GetS32("gTunic_Zora_R", 0)/255;
        zora_col[1] = (float)CVar_GetS32("gTunic_Zora_G", 60)/255;
        zora_col[2] = (float)CVar_GetS32("gTunic_Zora_B", 100)/255;
        navi_idle_i_col[0] = (float)CVar_GetS32("gNavi_Idle_Inner_R", 255)/255;
        navi_idle_i_col[1] = (float)CVar_GetS32("gNavi_Idle_Inner_G", 255)/255;
        navi_idle_i_col[2] = (float)CVar_GetS32("gNavi_Idle_Inner_B", 255)/255;
        navi_idle_o_col[0] = (float)CVar_GetS32("gNavi_Idle_Outer_R", 115)/255;
        navi_idle_o_col[1] = (float)CVar_GetS32("gNavi_Idle_Outer_G", 230)/255;
        navi_idle_o_col[2] = (float)CVar_GetS32("gNavi_Idle_Outer_B", 255)/255;
        navi_npc_i_col[0] = (float)CVar_GetS32("gNavi_NPC_Inner_R", 100)/255;
        navi_npc_i_col[1] = (float)CVar_GetS32("gNavi_NPC_Inner_G", 100)/255;
        navi_npc_i_col[2] = (float)CVar_GetS32("gNavi_NPC_Inner_B", 255)/255;
        navi_npc_o_col[0] = (float)CVar_GetS32("gNavi_NPC_Outer_R", 90)/255;
        navi_npc_o_col[1] = (float)CVar_GetS32("gNavi_NPC_Outer_G", 90)/255;
        navi_npc_o_col[2] = (float)CVar_GetS32("gNavi_NPC_Outer_B", 255)/255;
        navi_enemy_i_col[0] = (float)CVar_GetS32("gNavi_Enemy_Inner_R", 255)/255;
        navi_enemy_i_col[1] = (float)CVar_GetS32("gNavi_Enemy_Inner_G", 255)/255;
        navi_enemy_i_col[2] = (float)CVar_GetS32("gNavi_Enemy_Inner_B", 0)/255;
        navi_enemy_o_col[0] = (float)CVar_GetS32("gNavi_Enemy_Outer_R", 220)/255;
        navi_enemy_o_col[1] = (float)CVar_GetS32("gNavi_Enemy_Outer_G", 220)/255;
        navi_enemy_o_col[2] = (float)CVar_GetS32("gNavi_Enemy_Outer_B", 0)/255;
        navi_prop_i_col[0] = (float)CVar_GetS32("gNavi_Prop_Inner_R", 0)/255;
        navi_prop_i_col[1] = (float)CVar_GetS32("gNavi_Prop_Inner_G", 255)/255;
        navi_prop_i_col[2] = (float)CVar_GetS32("gNavi_Prop_Inner_B", 0)/255;
        navi_prop_o_col[0] = (float)CVar_GetS32("gNavi_Prop_Outer_R", 0)/255;
        navi_prop_o_col[1] = (float)CVar_GetS32("gNavi_Prop_Outer_G", 220)/255;
        navi_prop_o_col[2] = (float)CVar_GetS32("gNavi_Prop_Outer_B", 0)/255;
    }

    int ClampFloatToInt(float value, int min, int max){
        return fmin(fmax(value,min),max);
    }

    void Init(WindowImpl window_impl) {
        impl = window_impl;
        Game::LoadSettings();
        ImGuiContext* ctx = ImGui::CreateContext();
        ImGui::SetCurrentContext(ctx);
        io = &ImGui::GetIO();
        io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        if (UseViewports()) {
            io->ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
        }
        console->Init();
        ImGuiWMInit();
        ImGuiBackendInit();
        ModInternal::registerHookListener({ GFX_INIT, [](const HookEvent ev) {

            if (GlobalCtx2::GetInstance()->GetWindow()->IsFullscreen())
                ShowCursor(Game::Settings.debug.menu_bar, Dialogues::dLoadSettings);

            LoadTexture("Game_Icon", "assets/ship_of_harkinian/icons/gSohIcon.png");
            LoadTexture("A-Btn", "assets/ship_of_harkinian/buttons/ABtn.png");
            LoadTexture("B-Btn", "assets/ship_of_harkinian/buttons/BBtn.png");
            LoadTexture("L-Btn", "assets/ship_of_harkinian/buttons/LBtn.png");
            LoadTexture("R-Btn", "assets/ship_of_harkinian/buttons/RBtn.png");
            LoadTexture("Z-Btn", "assets/ship_of_harkinian/buttons/ZBtn.png");
            LoadTexture("Start-Btn", "assets/ship_of_harkinian/buttons/StartBtn.png");
            LoadTexture("C-Left", "assets/ship_of_harkinian/buttons/CLeft.png");
            LoadTexture("C-Right", "assets/ship_of_harkinian/buttons/CRight.png");
            LoadTexture("C-Up", "assets/ship_of_harkinian/buttons/CUp.png");
            LoadTexture("C-Down", "assets/ship_of_harkinian/buttons/CDown.png");

        } });

        ModInternal::registerHookListener({ CONTROLLER_READ, [](const HookEvent ev) {
            pads = static_cast<OSContPad*>(ev->baseArgs["cont_pad"]);
        } });
        Game::InitSettings();
        LoadCosmeticColors();
    }

    void Update(EventImpl event) {
        if (needs_save) {
            Game::SaveSettings();
            SaveCVars();
            needs_save = false;
        }
        ImGuiProcessEvent(event);
    }

#ifdef ENABLE_DX11
#define BindButton(btn, status) ImGui::Image(impl.backend == Backend::DX11 ? GetTextureByID(DefaultAssets[btn]->textureId) : (ImTextureID)(DefaultAssets[btn]->textureId), ImVec2(16.0f * scale, 16.0f * scale), ImVec2(0, 0), ImVec2(1.0f, 1.0f), ImVec4(255, 255, 255, (status) ? 255 : 0));
#else
#define BindButton(btn, status) ImGui::Image((ImTextureID)(DefaultAssets[btn]->textureId), ImVec2(16.0f * scale, 16.0f * scale), ImVec2(0, 0), ImVec2(1.0f, 1.0f), ImVec4(255, 255, 255, (status) ? 255 : 0));
#endif
    void BindAudioSlider(const char* name, const char* key, float defaultValue, SeqPlayers playerId) {
        float value = CVar_GetFloat(key, defaultValue);

        ImGui::Text(name, static_cast<int>(100 * value));
        if (ImGui::SliderFloat((std::string("##") + key).c_str(), &value, 0.0f, 1.0f, "", ImGuiSliderFlags_AlwaysClamp)) {
            const float volume = floorf(value * 100) / 100;
            CVar_SetFloat(key, volume);
            needs_save = true;
            Game::SetSeqPlayerVolume(playerId, volume);
        }
    }

    void EnhancementRadioButton(std::string text, std::string cvarName, int id) {
        int val = CVar_GetS32(cvarName.c_str(), 0);
        if (ImGui::RadioButton(text.c_str(), id==val)) {
            CVar_SetS32(cvarName.c_str(), (int)id);
            needs_save = true;
        }
    }
    
    void EnhancementColor3(std::string text, std::string cvarName, float ColorRGB[3]) {
        if (ImGui::ColorEdit3(text.c_str(), ColorRGB)) {
            CVar_SetS32((cvarName+"R").c_str(), ClampFloatToInt(ColorRGB[0]*255,0,255));
            CVar_SetS32((cvarName+"G").c_str(), ClampFloatToInt(ColorRGB[1]*255,0,255));
            CVar_SetS32((cvarName+"B").c_str(), ClampFloatToInt(ColorRGB[2]*255,0,255));
            needs_save = true;
        }
    }

    void EnhancementCheckbox(std::string text, std::string cvarName) {
        bool val = (bool)CVar_GetS32(cvarName.c_str(), 0);
        if (ImGui::Checkbox(text.c_str(), &val)) {
            CVar_SetS32(cvarName.c_str(), val);
            needs_save = true;
        }
    }

    void EnhancementSliderInt(std::string text, std::string id, std::string cvarName, int min, int max, std::string format) {
        int val = CVar_GetS32(cvarName.c_str(), 0);

        ImGui::Text(text.c_str(), val);

        if (ImGui::SliderInt(id.c_str(), &val, min, max, format.c_str())) 
        {
            CVar_SetS32(cvarName.c_str(), val);
            needs_save = true;
        }

        if (val < min)
        {
            val = min;
            CVar_SetS32(cvarName.c_str(), val);
            needs_save = true;
        }

        if (val > max)
        {
            val = max;
            CVar_SetS32(cvarName.c_str(), val);
            needs_save = true;
        }
    }

    void EnhancementSliderFloat(std::string text, std::string id, std::string cvarName, float min, float max, std::string format, float defaultValue) {
        float val = CVar_GetFloat(cvarName.c_str(), defaultValue);

        ImGui::Text(text.c_str(), static_cast<int>(100 * val));

        if (ImGui::SliderFloat(id.c_str(), &val, min, max, format.c_str()))
        {
            CVar_SetFloat(cvarName.c_str(), val);
            needs_save = true;
        }

        if (val < min)
        {
            val = min;
            CVar_SetFloat(cvarName.c_str(), val);
            needs_save = true;
        }

        if (val > max)
        {
            val = max;
            CVar_SetFloat(cvarName.c_str(), val);
            needs_save = true;
        }
    }

    void DrawMainMenuAndCalculateGameSize() {
        console->Update();
        ImGuiBackendNewFrame();
        ImGuiWMNewFrame();
        ImGui::NewFrame();
        const std::shared_ptr<Window> wnd = GlobalCtx2::GetInstance()->GetWindow();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBackground |
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_NoResize;
        if (UseViewports()) {
            window_flags |= ImGuiWindowFlags_NoBackground;
        }
        if (Game::Settings.debug.menu_bar) window_flags |= ImGuiWindowFlags_MenuBar;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(ImVec2(wnd->GetCurrentWidth(), wnd->GetCurrentHeight()));
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
        ImGui::Begin("Main - Deck", nullptr, window_flags);
        ImGui::PopStyleVar(3);

        ImVec2 top_left_pos = ImGui::GetWindowPos();

        const ImGuiID dockId = ImGui::GetID("main_dock");

        if (!ImGui::DockBuilderGetNode(dockId)) {
            ImGui::DockBuilderRemoveNode(dockId);
            ImGui::DockBuilderAddNode(dockId, ImGuiDockNodeFlags_NoTabBar);

            ImGui::DockBuilderDockWindow("OoT Master Quest", dockId);

            ImGui::DockBuilderFinish(dockId);
        }

        ImGui::DockSpace(dockId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

        if (ImGui::IsKeyPressed(TOGGLE_BTN)) {
            Game::Settings.debug.menu_bar = !Game::Settings.debug.menu_bar;
            needs_save = true;
            GlobalCtx2::GetInstance()->GetWindow()->dwMenubar = Game::Settings.debug.menu_bar;
            ShowCursor(Game::Settings.debug.menu_bar, Dialogues::dMenubar);
        }

        if (ImGui::BeginMenuBar()) {
            if (DefaultAssets.contains("Game_Icon")) {
                ImGui::SetCursorPos(ImVec2(5, 2.5f));
#ifdef ENABLE_DX11
                ImGui::Image(impl.backend == Backend::DX11 ? GetTextureByID(DefaultAssets["Game_Icon"]->textureId) : reinterpret_cast<ImTextureID>(DefaultAssets["Game_Icon"]->textureId), ImVec2(16.0f, 16.0f));
#else
                ImGui::Image(reinterpret_cast<ImTextureID>(DefaultAssets["Game_Icon"]->textureId), ImVec2(16.0f, 16.0f));
#endif
                ImGui::SameLine();
                ImGui::SetCursorPos(ImVec2(25, 0));
            }
            ImGui::Text("Shipwright");
            ImGui::Separator();

            if (ImGui::BeginMenu("Audio")) {
                EnhancementSliderFloat("Master Volume: %d %%", "##Master_Vol", "gGameMasterVolume", 0.0f, 1.0f, "", 1.0f);

                BindAudioSlider("Main Music Volume: %d %%", "gMainMusicVolume", 1.0f, SEQ_BGM_MAIN);
                BindAudioSlider("Sub Music Volume: %d %%", "gSubMusicVolume", 1.0f, SEQ_BGM_SUB);
                BindAudioSlider("Sound Effects Volume: %d %%", "gSFXMusicVolume", 1.0f, SEQ_SFX);
                BindAudioSlider("Fanfare Volume: %d %%", "gFanfareVolume", 1.0f, SEQ_FANFARE);

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Controller")) {
                EnhancementSliderFloat("Gyro Sensitivity: %d %%", "##GYROSCOPE", "gGyroSensitivity", 0.0f, 1.0f, "", 1.0f);
                if (ImGui::Button("Recalibrate Gyro")) {
                    Game::Settings.controller.gyroDriftX = 0;
                    Game::Settings.controller.gyroDriftY = 0;
                }
                ImGui::Separator();

                EnhancementSliderFloat("Rumble Strength: %d %%", "##RUMBLE", "gRumbleStrength", 0.0f, 1.0f, "", 1.0f);
                EnhancementCheckbox("Show Inputs", "gInputEnabled");
                EnhancementSliderFloat("Input Scale: %dx", "##Input", "gInputScale", 1, 3, "", 1);
                ImGui::Separator();

                EnhancementCheckbox("Dpad Support on Pause and File Select", "gDpadPauseName");
                EnhancementCheckbox("DPad Support in Ocarina and Text Choice", "gDpadOcarinaText");
                EnhancementCheckbox("DPad Support for Browsing Shop Items", "gDpadShop");
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Enhancements")) {

                ImGui::Text("Gameplay");
                ImGui::Separator();

                EnhancementSliderInt("Text Speed: %dx", "##TEXTSPEED", "gTextSpeed", 1, 5, "");
                EnhancementCheckbox("Skip Text", "gSkipText");
                EnhancementCheckbox("Minimal UI", "gMinimalUI");
                EnhancementCheckbox("MM Bunny Hood", "gMMBunnyHood");
                EnhancementCheckbox("Visual Stone of Agony", "gVisualAgony");

                EnhancementCheckbox("Always show dungeon entrances", "gAlwaysShowDungeonMinimapIcon");
                
                ImGui::Text("Graphics");
                ImGui::Separator();
                EnhancementCheckbox("N64 Mode", "gN64Mode");
                EnhancementCheckbox("Animated Link in Pause Menu", "gPauseLiveLink");
                EnhancementCheckbox("Disable LOD", "gDisableLOD");
                EnhancementCheckbox("Enable 3D Dropped items", "gNewDrops");
                EnhancementCheckbox("Dynamic Wallet Icon", "gDynamicWalletIcon");

                if (ImGui::BeginMenu("Fixes")) {
                    EnhancementCheckbox("Fix L&R Pause menu", "gUniformLR");
                    EnhancementCheckbox("Fix Dungeon entrances", "gFixDungeonMinimapIcon");
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Cheats")) {
                if (ImGui::BeginMenu("Infinite...")) {
                    EnhancementCheckbox("Money", "gInfiniteMoney");
                    EnhancementCheckbox("Health", "gInfiniteHealth");
                    EnhancementCheckbox("Ammo", "gInfiniteAmmo");
                    EnhancementCheckbox("Magic", "gInfiniteMagic");
                    EnhancementCheckbox("Nayru's Love", "gInfiniteNayru");
                    ImGui::EndMenu();
                }

                EnhancementCheckbox("No Clip", "gNoClip");
                EnhancementCheckbox("Climb Everything", "gClimbEverything");
                EnhancementCheckbox("Moon Jump on L", "gMoonJumpOnL");
                EnhancementCheckbox("Super Tunic", "gSuperTunic");
                EnhancementCheckbox("Easy ISG", "gEzISG");
                EnhancementCheckbox("Unrestricted Items", "gNoRestrictItems");
                EnhancementCheckbox("Freeze Time", "gFreezeTime");

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Graphics")) {
                HOOK(ImGui::MenuItem("Anti-aliasing", nullptr, &Game::Settings.graphics.show));
                ImGui::EndMenu();
            }

            if (CVar_GetS32("gHudColors", 1) ==0) {
                SelectedHUD = 0;
            } else if (CVar_GetS32("gHudColors", 1) == 1) {
                SelectedHUD = 1;
            } else if (CVar_GetS32("gHudColors", 1) == 2) {
                SelectedHUD = 2;
            }
            if (ImGui::BeginMenu("Cosmetics")) {
                if (ImGui::BeginMenu("Tunics")) {
                    EnhancementCheckbox("Custom colors on tunics", "gUseTunicsCol");
                    EnhancementColor3("Kokiri Tunic", "gTunic_Kokiri_", kokiri_col);
                    EnhancementColor3("Goron Tunic", "gTunic_Goron_", goron_col);
                    EnhancementColor3("Zora Tunic", "gTunic_Zora_", zora_col);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Navi")) {
                    EnhancementCheckbox("Custom colors for Navi", "gUseNaviCol");
                    EnhancementColor3("Navi Idle Inner", "gNavi_Idle_Inner_", navi_idle_i_col);
                    EnhancementColor3("Navi Idle Outer", "gNavi_Idle_Outer_", navi_idle_o_col);
                    ImGui::Separator();
                    EnhancementColor3("Navi NPC Inner", "gNavi_NPC_Inner_", navi_npc_i_col);
                    EnhancementColor3("Navi NPC Outer", "gNavi_NPC_Outer_", navi_npc_o_col);
                    ImGui::Separator();
                    EnhancementColor3("Navi Enemy Inner", "gNavi_Enemy_Inner_", navi_enemy_i_col);
                    EnhancementColor3("Navi Enemy Outer", "gNavi_Enemy_Outer_", navi_enemy_o_col);
                    ImGui::Separator();
                    EnhancementColor3("Navi Prop Inner", "gNavi_Prop_Inner_", navi_prop_i_col);
                    EnhancementColor3("Navi Prop Outer", "gNavi_Prop_Outer_", navi_prop_o_col);
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Interface")) {
                    EnhancementRadioButton("N64 Colors", "gHudColors", 0);
                    EnhancementRadioButton("Gamecube Colors", "gHudColors", 1);
                    EnhancementRadioButton("Custom Colors", "gHudColors", 2);
                    if (ImGui::BeginMenu("Edit HUD Colors")) {
                        if (ImGui::BeginMenu("Hearts")) {
                            EnhancementColor3("Hearts normals", "gCCHeartsPrim", hearts_colors);
                            EnhancementColor3("Hearts double def", "gDDCCHeartsPrim", hearts_dd_colors);
                            ImGui::EndMenu();
                        }
                        if (ImGui::BeginMenu("Buttons")) {
                            EnhancementColor3("A Buttons", "gCCABtnPrim", a_btn_colors);
                            EnhancementColor3("B Buttons", "gCCBBtnPrim", b_btn_colors);
                            EnhancementColor3("C Buttons", "gCCCBtnPrim", c_btn_colors);
                            EnhancementColor3("Start Buttons", "gCCStartBtnPrim", start_btn_colors);
                            ImGui::EndMenu();
                        }
                        if (ImGui::BeginMenu("Magic Bar")) {
                            EnhancementColor3("Magic bar borders", "gCCMagicBorderPrim", magic_border_colors);
                            EnhancementColor3("Magic bar main color", "gCCMagicPrim", magic_remaining_colors);
                            EnhancementColor3("Magic bar being used", "gCCMagicUsePrim", magic_use_colors);
                            ImGui::EndMenu();
                        }
                        if (ImGui::BeginMenu("Misc")) {
                            EnhancementColor3("Minimap color", "gCCMinimapPrim", minimap_colors);
                            EnhancementColor3("Rupee icon color", "gCCRupeePrim", rupee_colors);
                            ImGui::EndMenu();
                        }
                        ImGui::EndMenu();
                    }
                    HOOK(ImGui::MenuItem("Interface edit", nullptr, &Game::Settings.cosmetics.uiedit));
                    ImGui::EndMenu();
                }
            ImGui::EndMenu();
            }
            
            if (Game::Settings.cosmetics.uiedit) {
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
                ImGui::Begin("Interface modifier", nullptr, ImGuiWindowFlags_None);
                EnhancementCheckbox("Use margins", "gHUDMargins");
                EnhancementSliderInt("Top : %dx", "##UIMARGINT", "gHUDMargin_T", -20, 20, "");
                EnhancementSliderInt("Left: %dx", "##UIMARGINL", "gHUDMargin_L", -25, 25, "");
                EnhancementSliderInt("Right: %dx", "##UIMARGINR", "gHUDMargin_R", -25, 25, "");
                EnhancementSliderInt("Bottom: %dx", "##UIMARGINB", "gHUDMargin_B", -20, 20, "");
                ImGui::End();
                ImGui::PopStyleColor();
            }

            if (CVar_GetS32("gLanguages", 0) == 0) {
                SelectedLanguage = 0;
            } else if (CVar_GetS32("gLanguages", 0) == 1) {
                SelectedLanguage = 1;
            } else if (CVar_GetS32("gLanguages", 0) == 2) {
                SelectedLanguage = 2;
            }
            if (ImGui::BeginMenu("Languages")) {
                EnhancementRadioButton("English", "gLanguages", 0);
                EnhancementRadioButton("German", "gLanguages", 1);
                EnhancementRadioButton("French", "gLanguages", 2);
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Developer Tools")) {
                HOOK(ImGui::MenuItem("Stats", nullptr, &Game::Settings.debug.soh));
                HOOK(ImGui::MenuItem("Console", nullptr, &console->opened));
                //HOOK(ImGui::MenuItem("Zelda map select (Don't use yet)", nullptr, &Game::Settings.debug.mapselect));

                ImGui::Text("Debug");
                ImGui::Separator();
                EnhancementSliderInt("gDbgIntVal: %d", "##MapStage", "gDbgIntVal", 1, 99, "");
                EnhancementCheckbox("Hide build infos", "gBuildInfos");
                EnhancementCheckbox("Debug Mode (L + R + Z)", "gDebugEnabled");
                EnhancementCheckbox("Debug Camera (DPad < + R)", "gDebugCamera");
                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::End();

        if (Game::Settings.graphics.show) {
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
            ImGui::Begin("Anti-aliasing settings", nullptr, ImGuiWindowFlags_None);
            ImGui::Text("Internal Resolution:");
            ImGui::SliderInt("Mul", reinterpret_cast<int*>(&gfx_current_dimensions.internal_mul), 1, 8);
            ImGui::Text("MSAA:");
            ImGui::SliderInt("MSAA", reinterpret_cast<int*>(&gfx_msaa_level), 1, 8);
            ImGui::End();
            ImGui::PopStyleColor();
        }

        static SceneSelectList sScenes[] = {
            { " 1:Hyrule Field", 0x00CD },
            { " 2:Kakariko Village", 0x00DB },
            { " 3:Graveyard", 0x00E4 },
            { " 4:Zora's River", 0x00EA },
            { " 5:Kokiri Forest", 0x00EE },
            { " 6:Sacred Forest Meadow", 0x00FC },
            { " 7:Lake Hylia", 0x0102 },
            { " 8:Zora's Domain", 0x0108 },
            { " 9:Zora's Fountain", 0x010E },
            { "10:Gerudo Valley", 0x0117 },
            { "11:Lost Woods", 0x011E },
            { "12:Desert Colossus", 0x0123 },
            { "13:Gerudo's Fortress", 0x0129 },
            { "14:Haunted Wasteland", 0x0130 },
            { "15:Hyrule Castle", 0x0138 },
            { "16:Death Mountain Trail", 0x013D },
            { "17:Death Mountain Crater", 0x0147 },
            { "18:Goron City", 0x014D },
            { "19:Lon Lon Ranch", 0x0157 },
            { "20:Temple Of Time", 0x0053 },
            { "21:Chamber of Sages", 0x006B },
            { "22:Shooting Gallery", 0x003B },
            { "23:Castle Courtyard Game", 0x007A },
            { "24:Grave 1", 0x031C },
            { "25:Grave 2", 0x004B },
            { "26:Royal Family's Tomb", 0x002D },
            { "27:Great Fairy's Fountain (Din)", 0x0315 },
            { "28:Great Fairy's Fountain (Farore)", 0x036D },
            { "29:Great Fairy's Fountain (Nayru)", 0x0371 },
            { "30:Ganon's Tower - Collapsing", 0x043F },
            { "31:Castle Courtyard", 0x0400 },
            { "32:Fishing Pond", 0x045F },
            { "33:Bombchu Bowling Alley", 0x0507 },
            { "34:Lon Lon Ranch House", 0x004F },
            { "35:Lon Lon Ranch Silo", 0x05D0 },
            { "36:Guard House", 0x007E },
            { "37:Potion Shop", 0x0072 },
            { "38:Treasure Chest Game", 0x0063 },
            { "39:House Of Skulltula", 0x0550 },
            { "40:Entrance to Market", 0x0033 },
            { "41:Market", 0x00B1 },
            { "42:Back Alley", 0x00AD },
            { "43:Temple of Time Exterior", 0x0171 },
            { "44:Link's House", 0x00BB },
            { "45:Kakariko House 1", 0x02FD },
            { "46:Back Alley House 1", 0x043B },
            { "47:House of the Know-it-All Brothers", 0x00C9 },
            { "48:House of Twins", 0x009C },
            { "49:Mido's House", 0x0433 },
            { "50:Saria's House", 0x0437 },
            { "51:Stable", 0x02F9 },
            { "52:Grave Keeper's Hut", 0x030D },
            { "53:Dog Lady's House", 0x0398 },
            { "54:Impa's House", 0x039C },
            { "55:Lakeside Laboratory", 0x0043 },
            { "56:Running Man's Tent", 0x03A0 },
            { "57:Bazaar", 0x00B7 },
            { "58:Kokiri Shop", 0x00C1 },
            { "59:Goron Shop", 0x037C },
            { "60:Zora Shop", 0x0380 },
            { "61:Closed Shop", 0x0384 },
            { "62:Potion Shop", 0x0388 },
            { "63:Bombchu Shop ", 0x0390 },
            { "64:Happy Mask Shop", 0x0530 },
            { "65:Gerudo Training Ground", 0x0008 },
            { "66:Inside the Deku Tree", 0x0000 },
            { "67:Gohma's Lair", 0x040F },
            { "68:Dodongo's Cavern", 0x0004 },
            { "69:King Dodongo's Lair", 0x040B },
            { "70:Inside Jabu-Jabu's Belly", 0x0028 },
            { "71:Barinade's Lair", 0x0301 },
            { "72:Forest Temple", 0x0169 },
            { "73:Phantom Ganon's Lair", 0x000C },
            { "74:Bottom of the Well", 0x0098 },
            { "75:Shadow Temple", 0x0037 },
            { "76:Bongo Bongo's Lair", 0x0413 },
            { "77:Fire Temple", 0x0165 },
            { "78:Volvagia's Lair", 0x0305 },
            { "79:Water Temple", 0x0010 },
            { "80:Morpha's Lair", 0x0417 },
            { "81:Spirit Temple", 0x0082 },
            { "82:Iron Knuckle's Lair", 0x008D },
            { "83:Twinrova's Lair", 0x05EC },
            { "84:Stairs to Ganondorf's Lair", 0x041B },
            { "85:Ganondorf's Lair", 0x041F },
            { "86:Ice Cavern", 0x0088 },
            { "87:DampÃ© Grave Relay Game", 0x044F },
            { "88:Inside Ganon's Castle", 0x0467 },
            { "89:Ganon's Lair", 0x0517 },
            { "90:Escaping Ganon's Castle 1", 0x0179 },
            { "91:Escaping Ganon's Castle 2", 0x01B5 },
            { "92:Escaping Ganon's Castle 3", 0x03DC },
            { "93:Escaping Ganon's Castle 4", 0x03E4 },
            { "94:Escaping Ganon's Castle 5", 0x056C },
            { "95:Thieves' Hideout 1-2", 0x0486 },
            { "96:Thieves' Hideout 3-4 9-10", 0x048E },
            { "97:Thieves' Hideout 5-6", 0x0496 },
            { "98:Thieves' Hideout 7-8", 0x049E },
            { "99:Thieves' Hideout 11-12", 0x04AE },
            { "100:Thieves' Hideout 13", 0x0570 },
            { "101:Grotto 0", 0x003F },
            { "102:Grotto 1", 0x0598 },
            { "103:Grotto 2", 0x059C },
            { "104:Grotto 3", 0x05A0 },
            { "105:Grotto 4", 0x05A4 },
            { "106:Grotto 5", 0x05A8 },
            { "107:Grotto 6", 0x05AC },
            { "108:Grotto 7", 0x05B0 },
            { "109:Grotto 8", 0x05B4 },
            { "110:Grotto 9", 0x05B8 },
            { "111:Grotto 10", 0x05BC },
            { "112:Grotto 11", 0x05C0 },
            { "113:Grotto 12", 0x05C4 },
            { "114:Grotto 13", 0x05FC },
            { "115:Goddess Cutscene Environment", 0x00A0 },
            { "116:Test Room", 0x0520 },
            { "117:SRD Map", 0x0018 },
            { "118:Test Map", 0x0094 },
            { "119:Treasure Chest Warp", 0x0024 },
            { "120:Stalfos Miniboss Room", 0x001C },
            { "121:Stalfos Boss Room", 0x0020 },
            { "122:Dark Link Room", 0x0047 },
            { "123:Shooting Gallery Duplicate", 0x02EA },
            { "124:depth test", 0x00B6 },
            { "125:Hyrule Garden Game (Broken)", 0x0076 },
            { "title", 0x0000 }
        };

        if (Game::Settings.debug.mapselect) {
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0, 0, 0, 0));
            ImGui::Begin("Zelda map select", nullptr, ImGuiWindowFlags_None);
            ImVec2 ScrollListSize = ImGui::GetContentRegionAvail();
            ImGui::BeginListBox("Maps list",ImVec2(ScrollListSize.x,(ScrollListSize.y)-100));

            for (int i = 0; i <= 125 ; i++) {
                ImVec2 ButtonSize = ImGui::GetContentRegionAvail();
                if (ImGui::Button(sScenes[i].name.c_str(),ImVec2(ButtonSize.x,18))) {
                    
                    //gSaveContext.linkAge = CVar_GetS32("gDbgMapSelAge", 0);
                    int cutesceneindex[11] = {
                        0xFFF0,0xFFF1,0xFFF2,
                        0xFFF3,0xFFF4,0xFFF5,
                        0xFFF6,0xFFF7,0xFFF8,
                        0xFFF9,0xFFFA
                    };
                    //gSaveContext.cutsceneIndex = cutesceneindex[CVar_GetS32("gDbgMapSelStage", 0)];

                };
            }
            ImGui::EndListBox();
            EnhancementSliderInt("Stage: %d", "##MapStage", "gDbgMapSelStage", 0, 11, "");
            ImGui::Text("Age:");
            EnhancementRadioButton("Child", "gDbgMapSelAge", 0);
            EnhancementRadioButton("Adult", "gDbgMapSelAge", 1);
            ImGui::End();
            ImGui::PopStyleColor();
        }

        console->Draw();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
        ImGuiWindowFlags flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBackground;
        ImGui::Begin("OoT Master Quest", nullptr, flags);
        ImGui::PopStyleVar(3);
        ImGui::PopStyleColor();

        ImVec2 main_pos = ImGui::GetWindowPos();
        main_pos.x -= top_left_pos.x;
        main_pos.y -= top_left_pos.y;
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImVec2(0, 0);
        gfx_current_dimensions.width = size.x * gfx_current_dimensions.internal_mul;
        gfx_current_dimensions.height = size.y * gfx_current_dimensions.internal_mul;
        gfx_current_game_window_viewport.x = main_pos.x;
        gfx_current_game_window_viewport.y = main_pos.y;
        gfx_current_game_window_viewport.width = size.x;
        gfx_current_game_window_viewport.height = size.y;
        if (CVar_GetS32("gN64Mode", 0)) {
            gfx_current_dimensions.width = 320;
            gfx_current_dimensions.height = 240;
            const int sw = size.y * 320 / 240;
            gfx_current_game_window_viewport.x += (size.x - sw) / 2;
            gfx_current_game_window_viewport.width = sw;
            pos = ImVec2(size.x / 2 - sw / 2, 0);
            size = ImVec2(sw, size.y);
        }
    }

    void DrawFramebufferAndGameInput() {
        ImVec2 main_pos = ImGui::GetWindowPos();
        ImVec2 size = ImGui::GetContentRegionAvail();
        ImVec2 pos = ImVec2(0, 0);
        if (CVar_GetS32("gN64Mode", 0)) {
            const int sw = size.y * 320 / 240;
            pos = ImVec2(size.x / 2 - sw / 2, 0);
            size = ImVec2(sw, size.y);
        }
        std::string fb_str = SohUtils::getEnvironmentVar("framebuffer");
        if (!fb_str.empty()) {
            uintptr_t fbuf = (uintptr_t)std::stoull(fb_str);
            //ImGui::ImageSimple(reinterpret_cast<ImTextureID>(fbuf), pos, size);
            ImGui::SetCursorPos(pos);
            ImGui::Image(reinterpret_cast<ImTextureID>(fbuf), size);
        }

        ImGui::End();

        const float scale = CVar_GetFloat("gInputScale", 1.0f);
        ImVec2 BtnPos = ImVec2(160 * scale, 85 * scale);

        if (CVar_GetS32("gInputEnabled", 0)) {
            ImGui::SetNextWindowSize(BtnPos);
            ImGui::SetNextWindowPos(ImVec2(main_pos.x + size.x - BtnPos.x - 20, main_pos.y + size.y - BtnPos.y - 20));

            if (pads != nullptr && ImGui::Begin("Game Buttons", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBackground)) {
                ImGui::SetCursorPosY(32 * scale);

                ImGui::BeginGroup();
                const ImVec2 cPos = ImGui::GetCursorPos();
                ImGui::SetCursorPos(ImVec2(cPos.x + 10 * scale, cPos.y - 20 * scale));
                BindButton("L-Btn", pads[0].button & BTN_L);
                ImGui::SetCursorPos(ImVec2(cPos.x + 16 * scale, cPos.y));
                BindButton("C-Up", pads[0].button & BTN_CUP);
                ImGui::SetCursorPos(ImVec2(cPos.x, cPos.y + 16 * scale));
                BindButton("C-Left", pads[0].button & BTN_CLEFT);
                ImGui::SetCursorPos(ImVec2(cPos.x + 32 * scale, cPos.y + 16 * scale));
                BindButton("C-Right", pads[0].button & BTN_CRIGHT);
                ImGui::SetCursorPos(ImVec2(cPos.x + 16 * scale, cPos.y + 32 * scale));
                BindButton("C-Down", pads[0].button & BTN_CDOWN);
                ImGui::EndGroup();

                ImGui::SameLine();

                ImGui::BeginGroup();
                const ImVec2 sPos = ImGui::GetCursorPos();
                ImGui::SetCursorPos(ImVec2(sPos.x + 21, sPos.y - 20 * scale));
                BindButton("Z-Btn", pads[0].button & BTN_Z);
                ImGui::SetCursorPos(ImVec2(sPos.x + 22, sPos.y + 16 * scale));
                BindButton("Start-Btn", pads[0].button & BTN_START);
                ImGui::EndGroup();

                ImGui::SameLine();

                ImGui::BeginGroup();
                const ImVec2 bPos = ImGui::GetCursorPos();
                ImGui::SetCursorPos(ImVec2(bPos.x + 20 * scale, bPos.y - 20 * scale));
                BindButton("R-Btn", pads[0].button & BTN_R);
                ImGui::SetCursorPos(ImVec2(bPos.x + 12 * scale, bPos.y + 8 * scale));
                BindButton("B-Btn", pads[0].button & BTN_B);
                ImGui::SetCursorPos(ImVec2(bPos.x + 28 * scale, bPos.y + 24 * scale));
                BindButton("A-Btn", pads[0].button & BTN_A);
                ImGui::EndGroup();

                ImGui::End();
            }
        }
    }

    void Render() {

        ImGui::Render();
        ImGuiRenderDrawData(ImGui::GetDrawData());
        if (UseViewports()) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void CancelFrame() {
        ImGui::EndFrame();
        if (UseViewports()) {
            ImGui::UpdatePlatformWindows();
        }
    }

    void BindCmd(const std::string& cmd, CommandEntry entry) {
        console->Commands[cmd] = std::move(entry);
    }
}
