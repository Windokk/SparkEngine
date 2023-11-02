#include "../Libs/ImGui_Lib/imgui.h"
#include "../Libs/ImGui_Lib/imgui_internal.h"
#include <glm/vec3.hpp>
#include <string>

void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);