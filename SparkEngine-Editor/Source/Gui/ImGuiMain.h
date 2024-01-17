#ifndef GUI_MAIN_H
#define GUI_MAIN_H


#include <glm/gtx/matrix_decompose.hpp>
#include <typeinfo>

#include "Libraries/ImGui_Lib/imgui.h"
#include "Libraries/ImGui_Lib/imgui_impl_glfw.h"
#include "Libraries/ImGui_Lib/imgui_impl_opengl3.h"
#include "Libraries/ImGui_Lib/IconsFontAwesome5Pro.h"
#include "Libraries/ImGui_Lib/imgui_notify.h"
#include <Libraries/ImGui_Lib/tahoma.h>
#include <Libraries/ImGui_Lib/ImGuizmo.h>



#include "Utils/Engine/EngineUtils.h"
#include "Basic Rendering/Camera/Camera.h"
#include "Level Management/LevelLoader.h"
#include "ImGuiUtils.h"
#include "Level Management/LevelWriter.h"
#include "Utils/Engine/FilesManager.h"



class ImGuiMain {
public:
	ImGuiMain();
	void Load(GLFWwindow* window, ImGuiIO& io);
	void Draw(GLFWwindow* window, Camera& cam, LevelLoader& loader, int& selectedObjectID, ImGuiIO& io);
	void SetupImGuiStyle()
	{
		// Future Dark style by rewrking from ImThemes
		ImGuiStyle& style = ImGui::GetStyle();

		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(12.0f, 12.0f);
		style.WindowRounding = 0.0f;
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(20.0f, 20.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_None;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 0.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(6.0f,6.0f);
		style.FrameRounding = 0.0f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(12.0f, 6.0f);
		style.ItemInnerSpacing = ImVec2(6.0f, 3.0f);
		style.CellPadding = ImVec2(12.0f, 6.0f);
		style.IndentSpacing = 20.0f;
		style.ColumnsMinSpacing = 6.0f;
		style.ScrollbarSize = 12.0f;
		style.ScrollbarRounding = 0.0f;
		style.GrabMinSize = 12.0f;
		style.GrabRounding = 0.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabMinWidthForCloseButton = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.2745098173618317f, 0.3176470696926117f, 0.4509803950786591f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.0384313753247261f, 0.03627451211214066f, 0.0319607856869698f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 0.8f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.1568627506494522f, 0.168627455830574f, 0.1921568661928177f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.4980392158031464f, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.5372549295425415f, 0.5529412031173706f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 0.8f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.196078434586525f, 0.1764705926179886f, 0.5450980663299561f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.1568627506494522f, 0.1843137294054031f, 0.250980406999588f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.1158786714076996f, 0.1158790588378906f, 0.1158798336982727f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.1158786714076996f, 0.1158790588378906f, 0.1158798336982727f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.0784313753247261f, 0.08627451211214066f, 0.1019607856869698f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.5215686559677124f, 0.6000000238418579f, 0.7019608020782471f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.03921568766236305f, 0.9803921580314636f, 0.9803921580314636f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(1.0f, 0.2901960909366608f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.9960784316062927f, 0.4745098054409027f, 0.6980392336845398f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.0470588244497776f, 0.05490196123719215f, 0.07058823853731155f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.1176470592617989f, 0.1333333402872086f, 0.1490196138620377f, 1.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803921729326248f, 0.105882354080677f, 0.1215686276555061f, 1.0f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2352941185235977f, 0.2156862765550613f, 0.5960784554481506f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1, 0.5137255191802979f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.02866955757141113f, 0.02866887211799622f, 0.02866887211799622f, 1.0f);
		style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.06866955757141113f, 0.06866887211799622f, 0.06866887211799622f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.7866955757141113f, 0.7866887211799622f, 0.7866887211799622f, 0.8f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.06866955757141113f, 0.06866887211799622f, 0.06866887211799622f, 0.7f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.06866955757141113f, 0.06866887211799622f, 0.06866887211799622f, 0.7f);
	}

	

	int current_file = -1;
	
	const char* project_dir = "../SparkEngine-Core/assets/generated/test_project";
	const char* current_dir = project_dir;
	bool changed_dir = false;

	GLuint FileIconID;
	GLuint FolderIconID;
	GLuint FolderUpIconID;
	int width;
	int height;

	FilesManager manager = FilesManager();
	LevelWriter writer = LevelWriter();

	char scene_File_Name[500] = "";
	char scene_File_Path[500] = "";

	ImGuizmo::MODE currentGizmoMode;
	ImGuizmo::OPERATION currentGizmoOperation;
	ImVec2 viewportSize;
	ImVec2 viewportPos;
	ImVec2 viewportTextureSize;
	bool isHoverViewport;
	bool showCloseButton;
	ImFont* solid;
	ImFont* opensans;
	ImFont* solidBig;
	ImFont* opensansBig;
	bool showContentBrowser = true;
	bool showDetails = true;
	bool showViewport = true;
	bool showHierarchy = true;
	bool showCreditsWindow = false;
	bool showDocumentation = false;
	bool showNewDialog = false;
	bool showSaveScene = false;
	bool showNewScene = false;
	bool showSettings = false;
};

void SaveTextureToFile(GLuint textureId, int width, int height, const char* filename, ImGuiMain& gui);

#endif