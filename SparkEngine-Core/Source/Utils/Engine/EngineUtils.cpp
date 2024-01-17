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
	stbi_set_flip_vertically_on_load(true);

	// Load from file
	int image_width = 0;
	int image_height = 0;
	unsigned char* image_data = stbi_load(filename, &image_width, &image_height, NULL, 4);
	if (image_data == NULL)
		return false;

	stbi_set_flip_vertically_on_load(false);

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
	path = substr(path, "Binaries/windows-x86_64/Debug/SparkEngine-Editor");
	return path;
}

std::vector<std::variant<File, Folder>> ListFiles(char* directory)
{
	std::vector<std::variant<File, Folder>> files;

	for (auto& file : std::filesystem::directory_iterator(directory)) {

		if (file.is_directory()) {
			Folder new_folder;
			std::string name = file.path().filename().string();
			new_folder.name = _strdup(name.c_str());
			std::string path = file.path().string();
			path = replaceCharacters(path, '\\', '/');
			new_folder.filepath = _strdup(path.c_str());
			files.push_back(new_folder);
		}
		else {
			File new_file;
			new_file.name = _strdup(file.path().filename().string().c_str());
			const char* extension = _strdup(file.path().extension().string().c_str());
			new_file.name = _strdup(substr(new_file.name, (std::string)extension).c_str());

			new_file.extension = file.path().extension().string();

			std::string path = file.path().string();
			path = replaceCharacters(path, '\\', '/');
			path = substr(path, new_file.name + (std::string)extension);
			new_file.filepath = _strdup(path.c_str());
			if (file.path().extension().string() == ".sl") {
				new_file.type = LEVEL;
			}
			if (file.path().extension().string() == ".txt") {
				new_file.type = TEXT;
			}
			std::unordered_set<std::string> validCodeFormat = { ".scode", ".sc" };
			if (validCodeFormat.count(file.path().extension().string()) > 0) {
				new_file.type = CODE;
			}
			std::unordered_set<std::string> validImageFormat = { ".png", ".jpg" };
			if (validImageFormat.count(file.path().extension().string()) > 0) {
				new_file.type = TEXTURE;
			}
			std::unordered_set<std::string> validCPPFormat = { ".cpp", ".c++" };
			if (validCPPFormat.count(file.path().extension().string()) > 0) {
				new_file.type = CPP;
			}
			if (file.path().extension().string() == ".h") {
				new_file.type = H;
			}
			if (file.path().extension().string() == ".mat") {
				new_file.type = MATERIAL;
			}
			std::unordered_set<std::string> validShaderFormat = { ".frag", ".vert" };
			if (validShaderFormat.count(file.path().extension().string()) > 0) {
				new_file.type = SHADER;
			}
			new_file.id = files.size();
			files.push_back(new_file);
		}
		
	}

	return files;
}

char* extractPath(const char* path) {
	// Find the last occurrence of '/'
	const char* lastSlash = std::strrchr(path, '/');

	if (lastSlash != nullptr) {
		// Calculate the length of the substring up to the last '/'
		std::size_t length = lastSlash - path + 1;

		// Create a new char array to store the substring
		char* newPath = new char[length];

		// Copy the substring to the new char array using memcpy
		std::memcpy(newPath, path, length);

		// Null-terminate the new char array
		newPath[length - 1] = '\0';

		return newPath;
	}


	// Return a copy of the original path if '/' is not found
	return _strdup(path);
}