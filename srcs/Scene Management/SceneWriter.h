#include <iostream>
#include <fstream>
#include <json/json.h>

#include "SceneLoader.h"

using json = nlohmann::json;

class SceneWriter {
public:
	SceneWriter();
	void WriteSceneToFile(const char* filename, SceneLoader& loader);
	void WriteEmptySceneToFile(const char* filename);
};
