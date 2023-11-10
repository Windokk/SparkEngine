#include "SceneWriter.h"

SceneWriter::SceneWriter() {

}

void SceneWriter::WriteSceneToFile(const char* filename, SceneLoader& loader)
{
	// We initialize the json data
	json data;

	// We fill the json data
	data["name"] = "John Doe";
	data["age"] = 25;
	data["city"] = "Example City";

	// We write the data and export the file
	std::string jsonString = data.dump(4);

	std::ofstream outputFile;

	std::cout << filename;

	outputFile.open(filename);
	outputFile << jsonString;
	outputFile.close();
	

}
