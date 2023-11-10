#include "EngineUtils.h"

#include<stb/stb_image_write.h>

#include <stb/stb_image.h>


bool to_bool(std::string str) {
    bool b = true;
    if (str=="false") {
        b = false;
    }
    return b; 
}

float randf()
{
	//Generate a random float between 1 and -1
    return -1.0f + (rand() / (RAND_MAX / 2.0f));
}

std::string OpenWindowsFileDialog(LPCWSTR filters)
{
	OPENFILENAMEW ofn;               // Notice the "W" for wide-character version
	wchar_t szFile[260] = { 0 };     // Buffer for the wide-character file name

	// Initialize OPENFILENAMEW
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = filters;   // L prefix for wide-character strings
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

	// Show the Open dialog box
	if (GetOpenFileNameW(&ofn) == TRUE)   // Notice the "W" for wide-character version
	{
		// Convert the wide-character string to a narrow-character string
		int size_needed = WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, NULL, 0, NULL, NULL);
		std::string selectedFile(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, ofn.lpstrFile, -1, &selectedFile[0], size_needed, NULL, NULL);
		return selectedFile;
	}

	// Return an empty string if the user cancels the dialog
	return "";
}

std::string OpenFolderDialog()
{
	IFileDialog* pfd;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));

	if (SUCCEEDED(hr))
	{
		// Set options for the folder dialog
		DWORD dwOptions;
		hr = pfd->GetOptions(&dwOptions);

		if (SUCCEEDED(hr))
		{
			hr = pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

			if (SUCCEEDED(hr))
			{
				// Show the folder dialog
				hr = pfd->Show(NULL);

				if (SUCCEEDED(hr))
				{
					// Get the result (selected folder)
					IShellItem* psi;
					hr = pfd->GetResult(&psi);

					if (SUCCEEDED(hr))
					{
						PWSTR pszPath;
						hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);

						if (SUCCEEDED(hr))
						{
							int size_needed = WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, NULL, 0, NULL, NULL);
							std::string selectedFolder(size_needed, 0);
							WideCharToMultiByte(CP_UTF8, 0, pszPath, -1, &selectedFolder[0], size_needed, NULL, NULL);

							CoTaskMemFree(pszPath);
							psi->Release();
							pfd->Release();
							return selectedFolder;
						}
					}
				}
			}
		}

		pfd->Release();
	}

	// Return an empty string if something goes wrong
	return "";
}

std::string replaceCharacters(const std::string& input, char targetChar, char replacementChar) {
    std::string result = input;
    for (char& c : result) {
        if (c == targetChar) {
            c = replacementChar;
        }
    }
    return result;
}

void SetTextureAlphaToOne(GLuint texture)
{
	// Bind the texture to modify its parameters
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set swizzle mask for all channels to red, except for alpha
	GLint swizzleMask[] = { GL_ONE };
	glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, swizzleMask);

	// Unbind the texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height)
{
	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	// Create a OpenGL texture identifier
	GLuint image_texture;
	glGenTextures(1, &image_texture);
	glBindTexture(GL_TEXTURE_2D, image_texture);

	// Setup filtering parameters for display
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // This is required on WebGL for non power-of-two textures
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Same

	// Upload pixels into texture
#if defined(GL_UNPACK_ROW_LENGTH) && !defined(__EMSCRIPTEN__)
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#endif
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image_width, image_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_data);
	stbi_image_free(image_data);

	*out_texture = image_texture;
	*out_width = image_width;
	*out_height = image_height;

	return true;
}

GLuint LoadImageTexture(const char* path) {
	int my_image_width = 0;
	int my_image_height = 0;
	GLuint my_image_texture = 0;
	bool ret = LoadTextureFromFile(path, &my_image_texture, &my_image_width, &my_image_height);
	IM_ASSERT(ret);
	return my_image_texture;
}

unsigned int LoadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

std::string substr(const std::string& mainString, const std::string& subString) {
	std::string result = mainString;
	size_t pos = result.find(subString);

	if (pos != std::string::npos) {
		result.erase(pos, subString.length());
	}

	return result;
}

namespace fs = std::filesystem;

std::string make_relative_filepath(const std::string& absolute_path, const std::string& base_path) {
	fs::path absPath(absolute_path);
	fs::path basePath(base_path);

	fs::path relativePath = fs::relative(absPath, basePath);
	return relativePath.string();
}
std::string make_relative_folderpath(const std::string& absolute_folder_path, const std::string& base_folder_path) {
	fs::path absPath(absolute_folder_path);
	fs::path basePath(base_folder_path);

	fs::path relativePath = fs::relative(absPath, basePath);
	return relativePath.string();
}

std::string get_solution_path() {
	char buffer[MAX_PATH];
	GetModuleFileNameA(nullptr, buffer, MAX_PATH);
	std::string path(buffer);
	path = replaceCharacters(path, '\\', '/');
	path = substr(path, "x64/Debug/SparkEngineMaster.exe");
	return path;
}