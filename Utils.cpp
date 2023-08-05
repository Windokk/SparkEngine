#include "Utils.h"


bool to_bool(std::string str) {
    bool b = true;
    if (str=="false") {
        b = false;
    }
    return b;
}

float randf()
{
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