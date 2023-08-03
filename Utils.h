#include <sstream>
#include <string>
#include <iomanip>
#include <algorithm>
#include <cctype>
#include <Windows.h>
#include <commdlg.h>
#include <glad/glad.h>
#include <stb/stb_image.h>

bool to_bool(std::string str);
float randf();
std::string OpenWindowsFileDialog(LPCWSTR filters);
std::string replaceCharacters(const std::string& input, char targetChar, char replacementChar);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);