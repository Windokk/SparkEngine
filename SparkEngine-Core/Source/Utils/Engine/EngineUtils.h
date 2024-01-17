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
#include"../../Libraries/ImGui_Lib/imgui.h"
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glm/gtx/string_cast.hpp>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <filesystem>
#include <Shobjidl.h>
#include <variant>
#include <unordered_set>


bool to_bool(std::string str);
float randf();
std::string OpenWindowsFileDialog(LPCWSTR filters);
std::string OpenFolderDialog();
std::string replaceCharacters(const std::string& input, char targetChar, char replacementChar);
void SetTextureAlphaToOne(GLuint texture);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
unsigned int LoadTexture(char const* path);
std::string substr(const std::string& mainString, const std::string& subString);
std::string make_relative_filepath(const std::string& absolute_path, const std::string& base_path);
std::string make_relative_folderpath(const std::string& absolute_path, const std::string& base_path);
std::string get_solution_path();

#ifndef LIGHT_STRUCT_H
#define LIGHT_STRUCT_H

enum lightType {
	LT_DIRECTIONNAL,
	LT_POINT,
	LT_SPOT
};

struct light_Infos {
	lightType type;
	glm::vec3 position;
	glm::vec3 direction = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float constant = 1.0f;
	float linear = 0.09f;
	float quadratic = 0.032f;
	float cutOff = glm::cos(glm::radians(12.5f));
	float outerCutOff = glm::cos(glm::radians(15.0f));
	float intensity;
	glm::vec3 color;
};

#endif // LIGHT_STRUCT_H

#ifndef FILE_STRUCT_H
#define FILE_STRUCT_H

enum fileType {
	EMPTY,
	FOLDER,
	FONT,
	LEVEL,
	MODEL,
	CODE,
	CPP,
	H,
	MATERIAL,
	SHADER,
	TEXT,
	TEXTURE,
	UI
	
	
};

struct File {
	const char* name;
	const char* filepath;
	fileType type;
	int id;
	std::string extension;
	File() {
		name = "Untitled";
		filepath = "";
		type = EMPTY;
	}
};

struct Folder {
	const char* name;
	const char* filepath;
};

#endif // FILE_STRUCT_H

std::vector<std::variant<File, Folder>> ListFiles(char* directory);
char* extractPath(const char* path);