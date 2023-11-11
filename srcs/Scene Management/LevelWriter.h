#include <iostream>
#include <fstream>
#include <json/json.h>

#include "LevelLoader.h"

using json = nlohmann::json;

class LevelWriter {
public:
	LevelWriter();
	void WriteLevelToFile(const char* filename, LevelLoader& loader);
	void WriteEmptyLevelToFile(const char* filename);
};
