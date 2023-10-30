#include "../Libs/ImGui_Lib/imgui.h"
#include "../Libs/ImGui_Lib/imgui_impl_glfw.h"
#include "../Libs/ImGui_Lib/imgui_impl_opengl3.h"

#include "../Utils/Engine/EngineUtils.h"
#include "../Basic Rendering/Camera/Camera.h"
#include "../Scene Management/SceneLoader.h"

class ImGuiMain {
public:
	ImGuiMain();
	void Load(GLFWwindow* window, ImGuiIO& io);
	void Draw(GLFWwindow* window, Camera& cam, SceneLoader& loader, int& selectedObjectID, ImGuiIO& io);
	ImGuiWindowFlags windowFlags;
	bool isHoverViewport;
	bool showCloseButton;
	bool showRenameDialog = false;
};