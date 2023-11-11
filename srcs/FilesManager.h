#include "Utils/Engine/EngineUtils.h"

class FilesManager {
public :
	FilesManager();

	std::vector<std::variant<File, Folder>> files;

};
