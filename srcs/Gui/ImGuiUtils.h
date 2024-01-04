#include "../Libraries/ImGui_Lib/imgui.h"
#include "../Libraries/ImGui_Lib/imgui_internal.h"
#include <glm/vec3.hpp>
#include <string>
#include <vector>

void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);