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
#include<stb/stb_image_write.h>
#include <stb/stb_image.h>
#include"imgui/imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

bool to_bool(std::string str);
float randf();
std::string OpenWindowsFileDialog(LPCWSTR filters);
std::string replaceCharacters(const std::string& input, char targetChar, char replacementChar);
void SetTextureAlphaToOne(GLuint texture);
void SaveTextureToFile(GLuint textureId, int width, int height, const char* filename);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
GLuint LoadImageTexture(const char* path);
