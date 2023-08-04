#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <vector>
#include <algorithm>
#include <cctype>
#include <Windows.h>
#include <commdlg.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <imgui.h>
#include <imgui_internal.h>

bool to_bool(std::string str);
float randf();
std::string OpenWindowsFileDialog(LPCWSTR filters);
std::string replaceCharacters(const std::string& input, char targetChar, char replacementChar);

