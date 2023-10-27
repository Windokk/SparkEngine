#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "EngineUtils.h"
#include "Camera.h"
#include "SceneLoader.h"

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