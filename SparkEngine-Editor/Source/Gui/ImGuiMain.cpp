#include "ImGuiMain.h"

#ifndef STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include <stb/stb_image_resize.h>
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include<stb/stb_image_write.h>
#endif

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#endif


ImGuiMain::ImGuiMain()
{
}

void resizeImage(const char* inputPath, const char* outputPath, double targetAspectRatio) {
	// Load the image using stb_image
	int width, height, channels;
	unsigned char* imgData = stbi_load(inputPath, &width, &height, &channels, 0);

	if (imgData == nullptr) {
		std::cerr << "Error: Could not load the image." << std::endl;
		return;
	}

	// Get the current aspect ratio
	double currentAspectRatio = static_cast<double>(width) / static_cast<double>(height);

	// Calculate the new dimensions to match the target aspect ratio
	int newWidth, newHeight;
	if (currentAspectRatio > targetAspectRatio) {
		// Image is wider than the target aspect ratio, reduce its width
		newWidth = static_cast<int>(height * targetAspectRatio);
		newHeight = height;
	}
	else {
		// Image is taller than the target aspect ratio, reduce its height
		newWidth = width;
		newHeight = static_cast<int>(width / targetAspectRatio);
	}

	// Resize the image using stb_image_resize
	unsigned char* resizedImgData = new unsigned char[newWidth * newHeight * channels];
	stbir_resize_uint8(imgData, width, height, 0, resizedImgData, newWidth, newHeight, 0, channels);


	// Save the resized image using stb_image_write
	stbi_write_png(outputPath, newWidth, newHeight, channels, resizedImgData, newWidth * channels);

	// Free allocated memory
	stbi_image_free(imgData);
	delete[] resizedImgData;
}

void SaveTextureToFile(GLuint textureId, int width, int height, const char* filename, ImGuiMain& gui) {
	glBindTexture(GL_TEXTURE_2D, textureId);

	unsigned char* imageData = new unsigned char[width * height * 4];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	stbi_flip_vertically_on_write(true);
	stbi_write_png(filename, width, height, 4, imageData, width * 4);
	stbi_flip_vertically_on_write(false);
	resizeImage(filename, filename, gui.viewportTextureSize.x / gui.viewportTextureSize.y);

	delete[] imageData;

	glBindTexture(GL_TEXTURE_2D, 0);
}

void LoadLevelFromFile(LevelLoader& loader) {
	std::string file = OpenWindowsFileDialog(L"Spark Level file (.sl)\0*.sl\0");
	if (file != "") {
		file = replaceCharacters(file, '\\', '/');
		std::string file_relative;
		file_relative = make_relative_filepath(file, get_solution_path());
		loader.LoadNewLevel(file_relative.c_str());
	}
}

void ImGuiMain::Load(GLFWwindow* window, ImGuiIO& io)
{
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	io.ConfigFlags |= (ImGuiConfigFlags_NoMouseCursorChange, ImGuiConfigFlags_DockingEnable);
	ImGuiMain::SetupImGuiStyle();

	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;

	opensansBig = io.FontDefault = io.Fonts->AddFontFromFileTTF("../SparkEngine-Core/assets/defaults/gui/engine/fonts/OpenSans-Medium.ttf", 15.0f);
	solidBig = io.Fonts->AddFontFromFileTTF("../SparkEngine-Core/assets/defaults/gui/engine/fonts/fa-solid-900.ttf", 34.0f, &icons_config, icons_ranges);

	opensans_regular = io.FontDefault = io.Fonts->AddFontFromFileTTF("../SparkEngine-Core/assets/defaults/gui/engine/fonts/OpenSans-Medium.ttf", 13);
	regular = io.Fonts->AddFontFromFileTTF("../SparkEngine-Core/assets/defaults/gui/engine/fonts/fa-regular-400.ttf", 13.0f, &icons_config, icons_ranges);

	opensans = io.FontDefault = io.Fonts->AddFontFromFileTTF("../SparkEngine-Core/assets/defaults/gui/engine/fonts/OpenSans-Medium.ttf", 13);
	solid = io.Fonts->AddFontFromFileTTF("../SparkEngine-Core/assets/defaults/gui/engine/fonts/fa-solid-900.ttf", 13.0f, &icons_config, icons_ranges);


	ImFontConfig font_cfg;
	font_cfg.FontDataOwnedByAtlas = false;
	//io.Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &font_cfg);

	currentGizmoMode = ImGuizmo::WORLD;
	currentGizmoOperation = ImGuizmo::TRANSLATE;

	logo_textureID = LoadTexture("../SparkEngine-Core/assets/defaults/logos/icon.png");
}

void ImGuiMain::Draw(GLFWwindow* window, Camera& cam, LevelLoader& loader, int& selectedObjectID, ImGuiIO& io)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();



	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;
	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;

	ImGuiStyle& style = ImGui::GetStyle();

	//Teleports to selectedObject when pressing F
	if (io.KeysDown[GLFW_KEY_F]) {
		if (selectedObjectID != -1) {
			cam.Position = loader.objects_Transforms[selectedObjectID].Location;
		}
	}

	//Main Menu Bar
	if (ImGui::BeginMainMenuBar()) {

		
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);
		ImGui::Image((void*)(intptr_t)logo_textureID, ImVec2(25, 25));

		ImGui::SetCursorPosX(ImGui::GetCursorPosX() - 10);
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem(ICON_FA_FILE_PLUS "  New", "Ctrl+N")) {
				showNewDialog = true;
			}
			ImGui::Separator();
			if (ImGui::BeginMenu(ICON_FA_FILE_UPLOAD "  Import to project")) {
				if (ImGui::MenuItem(ICON_FA_CLAPPERBOARD "  Scene")) {
					LoadLevelFromFile(loader);
				}
				if (ImGui::MenuItem(ICON_FA_CUBE "  Model")) {
				}
				if (ImGui::MenuItem(ICON_FA_IMAGE"  Texture")) {
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem(ICON_FA_SAVE "  Save  file...", "Ctrl+S")) {

			}
			if (ImGui::MenuItem(ICON_FA_SAVE "  Save  scene", "Ctrl+Maj+S")) {
				if (current_file != -1) {
					std::string full_path = std::string(std::get<File>(manager.files[current_file]).filepath) + std::get<File>(manager.files[current_file]).name + ".sl";

					writer.WriteLevelToFile(full_path.c_str(), loader);
				}
				else {
					showSaveScene = true;
				}
			}
			if (ImGui::MenuItem(ICON_FA_FILE_EDIT "  Open", "Ctrl+O"))
			{
			}
			if (ImGui::MenuItem(ICON_FA_POWER_OFF "  Quit", "Alt+F4"))
			{
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Select all", "Ctrl+A")) {}
			if (ImGui::MenuItem("Redo", "Ctrl+Maj+Z")) {}
			if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
			if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
			if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
			if (ImGui::MenuItem("Duplicate", "Ctrl+D")) {}
			if (ImGui::MenuItem("Delete", "Suppr")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Editor Preferences")) {}
			if (ImGui::MenuItem("Project Settings"))
			{
				showSettings = true;
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
			if (ImGui::MenuItem(ICON_FA_CAMERA"  Viewport")) {
				showViewport = true;
			}
			if (ImGui::MenuItem(ICON_FA_FILE"  Content Browser")) {
				showContentBrowser = true;
			}
			if (ImGui::MenuItem(ICON_FA_LIST"  Details panel")) {
				showDetails = true;
			}
			if (ImGui::MenuItem(ICON_FA_CUBES"  Hierarchy")) {
				showHierarchy = true;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools")) {
			if (ImGui::MenuItem("Find item", "Ctrl+F")) {

			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Build")) {
			if (ImGui::MenuItem(ICON_FA_ARCHIVE "  Package Project")) {}
			if (ImGui::MenuItem(ICON_FA_GAMEPAD "  Package Game")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem(ICON_FA_QUESTION"  Wiki")) {
				system("start https://github.com/Windokk/SparkEngine/wiki");
			}
			if (ImGui::MenuItem(ICON_FA_CODE"  Engine Source code")) {
				system("start https://github.com/Windokk/SparkEngine");
			}
			if (ImGui::MenuItem(ICON_FA_BOOK"  Engine Documentation")) {
				showDocumentation = true;
			}
			if (ImGui::MenuItem(ICON_FA_LIST"  Credits")) {
				showCreditsWindow = true;
			}
			if (ImGui::MenuItem(ICON_FA_CERTIFICATE"  License")) {
				system("start https://github.com/Windokk/SparkEngine/blob/master/LICENSE");
			}
			ImGui::EndMenu();
		}

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 98);
		if (ImGui::Button(ICON_FA_MINUS"##minus_window_btn", ImVec2(25, 25))) {
			glfwIconifyWindow(window);
		}

		ImGui::PushFont(regular);
		if (ImGui::Button(ICON_FA_SQUARE"##max_window_btn", ImVec2(25, 25))) {
			glfwMaximizeWindow(window);
		}
		ImGui::PopFont();

		style.Colors[ImGuiCol_ButtonActive] = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(1.0f, 0.1f, 0.1f, 1.0f);
		if (ImGui::Button(ICON_FA_TIMES"##close_window_btn", ImVec2(25, 25))) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.26078434586525f, 0.26078434586525f, 0.26078434586525f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26078434586525f, 0.26078434586525f, 0.26078434586525f, 1.0f);

		ImGui::EndMainMenuBar();
	}

	//Settings Window
	if (showSettings) {
		ImGui::Begin("Project Settings", &showSettings);
		ImGui::BeginChild(1, ImVec2(250, 630), true);
		bool about = false;
		ImGui::Selectable("About", &about);
		bool publishing = false;
		ImGui::Selectable("Publishing", &publishing);
		bool legal = false;
		ImGui::Selectable("Legal", &legal);
		bool display = false;
		ImGui::Selectable("Display", &display);
		bool rules = false;
		ImGui::Selectable("Rules", &rules);
		ImGui::Separator();
		bool gameplay = false;
		ImGui::Selectable("Gameplay", &gameplay);

		ImGui::EndChild();
		ImGui::End();
	}

	//New file Popup
	if (showNewDialog) {
		ImGui::OpenPopup("Create a new file :  ");
		if (ImGui::BeginPopupModal("Create a new file :  ", &showNewDialog)) {

			ImGui::Button(ICON_FA_CLAPPERBOARD"  Scene");
			if (ImGui::IsItemClicked()) {
				showNewScene = true;
				showNewDialog = false;
			}


			if (io.KeysDown[GLFW_KEY_ENTER]) {
				showNewDialog = false;
			}
			ImGui::EndPopup();
		}
	}

	//New Scene Popup
	if (showNewScene) {
		ImGui::OpenPopup("Create Scene");
		if (ImGui::BeginPopupModal("Create Scene", &showNewScene)) {

			ImGui::InputTextWithHint("New Scene Name", "Enter the scene's name", scene_File_Name, sizeof(scene_File_Name));
			ImGui::Spacing();
			ImGui::InputTextWithHint("New Scene Path", "Enter the scene file's path", scene_File_Path, sizeof(scene_File_Path));
			ImGui::SameLine();
			ImGui::Button(ICON_FA_SEARCH " Browse");
			if (ImGui::IsItemClicked()) {
				std::string folder = OpenFolderDialog();
				if (folder != "") {
					std::string folder_relative;
					folder_relative = make_relative_folderpath(folder, get_solution_path());
					folder_relative = replaceCharacters(folder_relative, '\\', '/');
					folder_relative += "/";
					strcpy_s(scene_File_Path, folder_relative.c_str());
				}
			}
			ImGui::Spacing();
			ImGui::Button(ICON_FA_PLUS "  Create and Save");
			if (scene_File_Name != "" && scene_File_Path != "")
				if (io.KeysDown[GLFW_KEY_ENTER] || ImGui::IsItemClicked()) {
					File new_scene = File();
					new_scene.name = scene_File_Name;
					new_scene.filepath = scene_File_Path;
					new_scene.type = LEVEL;
					new_scene.id = manager.files.size() - 1;
					current_file = new_scene.id;
					manager.files.push_back(new_scene);
					std::string full_path = std::string(std::get<File>(manager.files[current_file]).filepath) + std::get<File>(manager.files[current_file]).name + ".sl";
					writer.WriteEmptyLevelToFile(full_path.c_str());
					showNewScene = false;
				}
			ImGui::EndPopup();
		}
	}

	//Save Scene Popup
	if (showSaveScene) {
		ImGui::OpenPopup("Save Scene");
		if (ImGui::BeginPopupModal("Save Scene", &showSaveScene)) {

			ImGui::InputTextWithHint("Scene Name", "Enter the scene's name", scene_File_Name, sizeof(scene_File_Name));
			ImGui::Spacing();
			ImGui::InputTextWithHint("Scene Path", "Enter the scene file's path", scene_File_Path, sizeof(scene_File_Path));
			ImGui::SameLine();
			ImGui::Button(ICON_FA_SEARCH " Browse");
			if (ImGui::IsItemClicked()) {
				std::string folder = OpenFolderDialog();
				if (folder != "") {
					std::string folder_relative;
					folder_relative = make_relative_folderpath(folder, get_solution_path());
					folder_relative = replaceCharacters(folder_relative, '\\', '/');
					folder_relative += "/";
					strcpy_s(scene_File_Path, folder_relative.c_str());
				}
			}
			ImGui::Spacing();
			ImGui::Button(ICON_FA_PLUS "  Create and Save");
			if (scene_File_Name != "" && scene_File_Path != "")
				if (io.KeysDown[GLFW_KEY_ENTER] || ImGui::IsItemClicked()) {
					File new_scene = File();
					new_scene.name = scene_File_Name;
					new_scene.filepath = scene_File_Path;
					new_scene.type = LEVEL;
					new_scene.id = manager.files.size();
					current_file = new_scene.id;
					manager.files.push_back(new_scene);
					std::string full_path = std::string(std::get<File>(manager.files[current_file]).filepath) + std::get<File>(manager.files[current_file]).name + ".sl";
					writer.WriteLevelToFile(full_path.c_str(), loader);
					showSaveScene = false;
				}
			ImGui::EndPopup();
		}
	}

	//Credits
	if (showCreditsWindow) {
		ImGui::Begin("Credits", &showCreditsWindow);

		ImGui::Text("Resources Used : ");
		ImGui::NewLine();
		ImGui::Text("Open GL with GLFW(context and window management) : ");
		ImGui::SameLine();
		if (ImGui::Button("GLFW")) {
			system("start https://www.glfw.org/");
		}
		///////////
		ImGui::Text("Open GL with Glad (OpenGL bindings and graphic card support) : ");
		ImGui::SameLine();
		if (ImGui::Button("GLAD")) {
			system("start https://glad.dav1d.de/");
		}
		///////////
		ImGui::Text("GLM (mathemical functions and types) : ");
		ImGui::SameLine();
		if (ImGui::Button("GLM")) {
			system("start https://github.com/g-truc/glm");
		}
		///////////
		ImGui::Text("Dear ImGui (all UI) : ");
		ImGui::SameLine();
		if (ImGui::Button("ImGui")) {
			system("start https://github.com/ocornut/imgui");
		}
		///////////
		ImGui::Text("STB Image (Image loading and management) : ");
		ImGui::SameLine();
		if (ImGui::Button("STB")) {
			system("start https://github.com/nothings/stb");
		}
		///////////
		ImGui::Text("ImGuizmo (3D Guizmos (Location, Rotation, Scale) : ");
		ImGui::SameLine();
		if (ImGui::Button("ImGuizmo")) {
			system("start https://github.com/CedricGuillemet/ImGuizmo");
		}

		ImGui::NewLine();
		ImGui::Text("Authors : ");

		ImGui::Text("Windokk : ");
		ImGui::SameLine();
		if (ImGui::Button("Windokk's Github Profile")) {
			system("start https://github.com/Windokk/");
		}

		ImGui::End();
	}

	//Documentation
	if (showDocumentation) {
		ImGui::SetNextWindowSize(ImVec2(260, 110));
		ImGui::Begin("Documentation", &showDocumentation, ImGuiWindowFlags_NoResize);

		auto windowWidth = ImGui::GetWindowSize().x;
		auto textWidth = ImGui::CalcTextSize(std::string("Documentation for Developpers").c_str()).x;
		ImGui::Spacing();
		ImGui::SetCursorPosX((windowWidth - textWidth) * 0.5f);
		bool DevlinkSelected = false;
		ImGui::Selectable(std::string("Documentation for Developpers").c_str(), &DevlinkSelected, ImGuiSelectableFlags_None, ImVec2(textWidth, 0));
		if (DevlinkSelected) {
			system("start https://github.com/Windokk/SparkEngine/blob/master/docs/README.md");
		}

		ImGui::NewLine();

		auto textWidth2 = ImGui::CalcTextSize(std::string("Documentation for Games Creators").c_str()).x;
		ImGui::SetCursorPosX((windowWidth - textWidth2) * 0.5f);
		bool GamelinkSelected = false;
		ImGui::Selectable(std::string("Documentation for Games Creators").c_str(), &GamelinkSelected, ImGuiSelectableFlags_None, ImVec2(textWidth2, 0));
		if (GamelinkSelected) {
			system("start https://github.com/Windokk/SparkEngine/wiki");
		}

		ImGui::End();
	}

	//Viewport
	if (showViewport) {
		style.WindowPadding = ImVec2(0, 0);
		ImGui::Begin(ICON_FA_CAMERA"  Viewport", &showViewport);

		//Viewport variables
		viewportPos = ImGui::GetWindowPos();
		viewportSize = ImGui::GetWindowSize();

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		viewManipulateRight = ImGui::GetWindowPos().x + ImGui::GetContentRegionAvail().x;
		viewManipulateTop = ImGui::GetWindowPos().y;

		const float* cameraView = glm::value_ptr(cam.view);
		const float* cameraProjection = glm::value_ptr(cam.projection);

		viewportTextureSize = ImGui::GetContentRegionAvail();
		ImGui::Image((void*)(intptr_t)loader.framebufferTexture, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		isHoverViewport = (ImGui::IsItemHovered());

		if (isHoverViewport && ImGui::IsWindowDocked() && !ImGuizmo::IsUsingAny()) {
			cam.Inputs(window, 0.01F, 100.0f, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2), ImGui::GetWindowSize());
		}
		if (!isHoverViewport) {
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange, ImGuiConfigFlags_DockingEnable;
		}
		if (selectedObjectID != -1) {
			TransformComponent& transform = loader.parser.objects[selectedObjectID].GetComponent<TransformComponent>();

			glm::mat4 transformMat = glm::mat4(1.0f);
			transformMat = glm::translate(glm::mat4(1.0f), loader.objects_Transforms[selectedObjectID].Location) * glm::toMat4(loader.objects_Transforms[selectedObjectID].Rotation) * glm::scale(glm::mat4(1.0f), glm::vec3(loader.objects_Transforms[selectedObjectID].Scale.x * 0.5, loader.objects_Transforms[selectedObjectID].Scale.y * 0.5, loader.objects_Transforms[selectedObjectID].Scale.z * 0.5));

			ImGuizmo::Manipulate(cameraView, cameraProjection, currentGizmoOperation, currentGizmoMode, glm::value_ptr(transformMat), NULL, useSnap ? &snap[0] : NULL, boundSizing ? bounds : NULL, boundSizingSnap ? boundsSnap : NULL);

			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(transformMat, transform.Scale, transform.Rotation, transform.Location, skew, perspective);

			transform.Scale /= 0.5;


			loader.objects_Transforms[selectedObjectID].Location = transform.Location;
			loader.objects_Transforms[selectedObjectID].Rotation = transform.Rotation;
			loader.objects_Transforms[selectedObjectID].Scale = transform.Scale;
		}



		/// Viewport Actions
		ImGui::SetCursorPosY(30);
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10);
		ImGui::Button(ICON_FA_CUBE "  Perspective"); //TODO : Change "perspective" to the camera perspective value
		ImGui::SameLine();
		ImGui::SetCursorPosX(viewportSize.x / 2);
		ImGui::Button(ICON_FA_PLAY);
		ImGui::SameLine();
		ImGui::SetCursorPosX(viewportSize.x - 110);
		ImGui::Button(ICON_FA_ARROWS);
		if (ImGui::IsItemClicked()) {
			currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		}
		ImGui::SameLine();
		ImGui::Button(ICON_FA_SYNC);
		if (ImGui::IsItemClicked()) {
			currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
		}
		ImGui::SameLine();
		ImGui::Button(ICON_FA_EXTERNAL_LINK);
		if (ImGui::IsItemClicked()) {
			currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
		}

		ImGui::End();
		style.WindowPadding = ImVec2(8, 8);

		if (io.KeysDown[GLFW_KEY_T]) {
			currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		}

		if (io.KeysDown[GLFW_KEY_R]) {
			currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
		}

		if (io.KeysDown[GLFW_KEY_S] && !isHoverViewport) {
			currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
		}
	}

	//Hierarchy
	if (showHierarchy) {
		ImGui::Begin(ICON_FA_CUBES"  Hierarchy", &showHierarchy);
		for (int i = 0; i < loader.parser.objects.size(); i++) {
			std::string spacing = "  ";
			std::string iconString;
			if (loader.parser.objects[i].HasComponent<LightComponent>()) {
				iconString = ICON_FA_LIGHTBULB + spacing + loader.parser.objects[i].name;
			}
			if (loader.parser.objects[i].HasComponent<ModelComponent>()) {
				iconString = ICON_FA_CUBE + spacing + loader.parser.objects[i].name;
			}

			if (ImGui::Selectable(iconString.c_str())) {
				selectedObjectID = i;
			}
			ImGui::Separator();
		}
		ImGui::End();
	}

	//Details
	if (showDetails) {
		ImGui::Begin(ICON_FA_LIST"  Details", &showDetails);
		ImGui::Text("Name : ");
		ImGui::SameLine();
		if (selectedObjectID != -1) {
			static char buffer[256];
			strcpy_s(buffer, loader.parser.objects[selectedObjectID].name.c_str());
			style.FrameRounding = 6.0f;
			ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
			ImGui::InputText("##ObjectName", buffer, 600);
			style.FrameRounding = 0.0f;
			ImGui::Separator();
			if (loader.parser.objects[selectedObjectID].HasComponent<TransformComponent>()) {
					Transform transform = loader.objects_Transforms[selectedObjectID];
					if (ImGui::CollapsingHeader("Transform")) {

						//////////////////////////////////////////////
						///////////  LOCATION  ///////////////////////
						//////////////////////////////////////////////

						DrawVec3Control("Location", transform.Location);
						ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 3.0f));

						//////////////////////////////////////////////
						///////////  ROTATION ////////////////////////
						//////////////////////////////////////////////

						// Extract axis and angle from the quaternion



						glm::vec3 angle = glm::degrees(glm::eulerAngles(transform.Rotation));
						// Modify the axis and angle values
						glm::vec3 axisAngle = angle;
						DrawVec3Control("Rotation", axisAngle);
						// Calculate the resulting vector from the modified axis and angle
						glm::vec3 newAxis = glm::normalize(axisAngle);
						float newAngle = glm::length(axisAngle);
						// Convert the modified vector back to a quaternion
						if (glm::length(newAxis) > 0) {

							transform.Rotation = glm::angleAxis(glm::radians(newAngle), newAxis);
						}
						else {
							transform.Rotation = glm::quat(1, 0, 0, 0); // Default to identity quaternion if the vector is zero
						}

						//////////////////////////////////////////////
						////////////////  SCALE   ////////////////////
						//////////////////////////////////////////////

						ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 3.0f));
						DrawVec3Control("Scale", transform.Scale);
					}
					// Reapply the new transform to the selected Object transform
					loader.objects_Transforms[selectedObjectID] = transform;
					ImGui::Spacing();
			}
			if (loader.parser.objects[selectedObjectID].HasComponent<ModelComponent>()) {
					if (ImGui::CollapsingHeader("Model")) {
						ModelComponent model = loader.parser.objects[selectedObjectID].GetComponent<ModelComponent>();
						char ModelPathbuffer[255]{};
						ImGui::Text("Model Path : ");
						ImGui::SameLine();
						ImGui::InputTextWithHint("##ModelPath", model.model_path.c_str(), ModelPathbuffer, sizeof(ModelPathbuffer));
						char Shaderbuffer[255]{};
						ImGui::Text("Shader :         ");
						ImGui::SameLine();
						ImGui::InputTextWithHint("##Shader", model.shader.c_str(), Shaderbuffer, sizeof(Shaderbuffer));
					}
				}
			if (loader.parser.objects[selectedObjectID].HasComponent<LightComponent>()) {
				if (ImGui::CollapsingHeader("Light")) {
					ImGui::Separator();
					LightComponent light = loader.parser.objects[selectedObjectID].GetComponent<LightComponent>();
					switch (light.type) {
					case LT_DIRECTIONNAL:
						ImGui::Text("Light Type :   Directionnal");
						//Direction
						ImGui::Text("Direction :   ");
						ImGui::SameLine();
						static float Direction[3];
						Direction[0] = light.direction.x;
						Direction[1] = light.direction.y;
						Direction[2] = light.direction.z;
						ImGui::InputFloat3("##direction", Direction);
						light.direction.x = Direction[0];
						light.direction.y = Direction[1];
						light.direction.z = Direction[2];
						//Ambient
						ImGui::Text("Ambient :     ");
						ImGui::SameLine();
						static float Ambient[3];
						Ambient[0] = light.ambient.x;
						Ambient[1] = light.ambient.y;
						Ambient[2] = light.ambient.z;
						ImGui::InputFloat3("##ambient", Ambient);
						light.ambient.x = Ambient[0];
						light.ambient.y = Ambient[1];
						light.ambient.z = Ambient[2];
						//Diffuse
						ImGui::Text("Diffuse :       ");
						ImGui::SameLine();
						static float Diffuse[3];
						Diffuse[0] = light.diffuse.x;
						Diffuse[1] = light.diffuse.y;
						Diffuse[2] = light.diffuse.z;
						ImGui::InputFloat3("##diffuse", Diffuse);
						light.diffuse.x = Diffuse[0];
						light.diffuse.y = Diffuse[1];
						light.diffuse.z = Diffuse[2];
						//Specular
						ImGui::Text("Specular :     ");
						ImGui::SameLine();
						static float Specular[3];
						Specular[0] = light.specular.x;
						Specular[1] = light.specular.y;
						Specular[2] = light.specular.z;
						ImGui::InputFloat3("##specular", Specular);
						light.specular.x = Specular[0];
						light.specular.y = Specular[1];
						light.specular.z = Specular[2];
						break;
					case LT_POINT:
						ImGui::Text("Light Type :   Point");
						//Ambient
						ImGui::Text("Ambient :     ");
						ImGui::SameLine();
						Ambient[3];
						Ambient[0] = light.ambient.x;
						Ambient[1] = light.ambient.y;
						Ambient[2] = light.ambient.z;
						ImGui::InputFloat3("##ambient", Ambient);
						light.ambient.x = Ambient[0];
						light.ambient.y = Ambient[1];
						light.ambient.z = Ambient[2];
						//Diffuse
						ImGui::Text("Diffuse :       ");
						ImGui::SameLine();
						Diffuse[3];
						Diffuse[0] = light.diffuse.x;
						Diffuse[1] = light.diffuse.y;
						Diffuse[2] = light.diffuse.z;
						ImGui::InputFloat3("##diffuse", Diffuse);
						light.diffuse.x = Diffuse[0];
						light.diffuse.y = Diffuse[1];
						light.diffuse.z = Diffuse[2];
						//Specular
						ImGui::Text("Specular :     ");
						ImGui::SameLine();
						Specular[3];
						Specular[0] = light.specular.x;
						Specular[1] = light.specular.y;
						Specular[2] = light.specular.z;
						ImGui::InputFloat3("##specular", Specular);
						light.specular.x = Specular[0];
						light.specular.y = Specular[1];
						light.specular.z = Specular[2];
						//Constant
						ImGui::Text(" Constant :    ");
						ImGui::SameLine();
						static float constant;
						constant = light.constant;
						ImGui::SameLine();
						ImGui::InputFloat("##constant", &constant);
						light.constant = constant;
						//Linear
						ImGui::Text(" Linear :      ");
						ImGui::SameLine();
						static float linear;
						linear = light.linear;
						ImGui::SameLine();
						ImGui::InputFloat("##linear", &linear);
						light.linear = linear;
						//Quadratic
						ImGui::Text(" Quadratic :   ");
						ImGui::SameLine();
						static float quadratic;
						quadratic = light.quadratic;
						ImGui::SameLine();
						ImGui::InputFloat("##quadratic", &quadratic);
						light.quadratic = quadratic;
						break;
					case LT_SPOT:
						ImGui::Text("Light Type :   Spot");
						//Direction
						ImGui::Text("Direction :   ");
						ImGui::SameLine();
						Direction[3];
						Direction[0] = light.direction.x;
						Direction[1] = light.direction.y;
						Direction[2] = light.direction.z;
						ImGui::InputFloat3("##direction", Direction);
						light.direction.x = Direction[0];
						light.direction.y = Direction[1];
						light.direction.z = Direction[2];
						//Ambient
						ImGui::Text("Ambient :     ");
						ImGui::SameLine();
						Ambient[3];
						Ambient[0] = light.ambient.x;
						Ambient[1] = light.ambient.y;
						Ambient[2] = light.ambient.z;
						ImGui::InputFloat3("##ambient", Ambient);
						light.ambient.x = Ambient[0];
						light.ambient.y = Ambient[1];
						light.ambient.z = Ambient[2];
						//Diffuse
						ImGui::Text("Diffuse :       ");
						ImGui::SameLine();
						Diffuse[3];
						Diffuse[0] = light.diffuse.x;
						Diffuse[1] = light.diffuse.y;
						Diffuse[2] = light.diffuse.z;
						ImGui::InputFloat3("##diffuse", Diffuse);
						light.diffuse.x = Diffuse[0];
						light.diffuse.y = Diffuse[1];
						light.diffuse.z = Diffuse[2];
						//Specular
						ImGui::Text("Specular :     ");
						ImGui::SameLine();
						Specular[3];
						Specular[0] = light.specular.x;
						Specular[1] = light.specular.y;
						Specular[2] = light.specular.z;
						ImGui::InputFloat3("##specular", Specular);
						light.specular.x = Specular[0];
						light.specular.y = Specular[1];
						light.specular.z = Specular[2];
						//Constant
						ImGui::Text(" Constant :    ");
						ImGui::SameLine();
						constant;
						constant = light.constant;
						ImGui::SameLine();
						ImGui::InputFloat("##constant", &constant);
						light.constant = constant;
						//Linear
						ImGui::Text(" Linear :      ");
						ImGui::SameLine();
						linear;
						linear = light.linear;
						ImGui::SameLine();
						ImGui::InputFloat("##linear", &linear);
						light.linear = linear;
						//Quadratic
						ImGui::Text(" Quadratic :   ");
						ImGui::SameLine();
						quadratic;
						quadratic = light.quadratic;
						ImGui::SameLine();
						ImGui::InputFloat("##quadratic", &quadratic);
						light.quadratic = quadratic;
						//CutOff
						ImGui::Text(" CutOff :    ");
						ImGui::SameLine();
						static float cutoff;
						cutoff = light.cutOff;
						ImGui::SameLine();
						ImGui::InputFloat("##cutoff", &cutoff);
						light.cutOff = cutoff;
						//OuterCutOff
						ImGui::Text(" OuterCutOff :    ");
						ImGui::SameLine();
						static float outercutoff;
						outercutoff = light.outerCutOff;
						ImGui::SameLine();
						ImGui::InputFloat("##outercutoff", &outercutoff);
						light.outerCutOff = outercutoff;
						break;
					}
					//Intensity
					static float intensity;
					intensity = light.intensity;
					ImGui::Text("Intensity :     ");
					ImGui::SameLine();
					ImGui::InputFloat("##Intensity", &intensity);
					light.intensity = intensity;
					//Color
					ImGui::Text("Light Color : ");
					ImGui::SameLine();
					float light_color[3];
					light_color[0] = light.color.x;
					light_color[1] = light.color.y;
					light_color[2] = light.color.z;
					ImGui::ColorEdit3("##LightColor", light_color, ImGuiColorEditFlags_DisplayRGB);
					light.color = glm::vec3(light_color[0], light_color[1], light_color[2]);

					Light_Object_Infos infos;
					infos.type = light.type;
					infos.objectID = selectedObjectID;
					infos.lightPos = loader.objects_Transforms[selectedObjectID].Location;
					infos.lightDirection = light.direction;
					infos.ambient = light.ambient;
					infos.diffuse = light.diffuse;
					infos.specular = light.specular;
					infos.constant = light.constant;
					infos.linear = light.linear;
					infos.quadratic = light.quadratic;
					infos.cutOff = light.cutOff;
					infos.outerCutOff = light.outerCutOff;
					infos.lightIntensity = light.intensity;
			   		infos.lightColor = light.color;
					infos.lightModel = glm::translate(glm::mat4(1.0f), infos.lightPos);
					for (int x = 0; x < loader.light_object_infos.size(); x++) {
						if (loader.light_object_infos[x].objectID == selectedObjectID) {
							loader.light_object_infos[x] = infos;
						}
					}
					for (int x = 0; x < loader.parser.objects[selectedObjectID].components.size(); x++) {
						if (std::holds_alternative<LightComponent>(loader.parser.objects[selectedObjectID].components[x])) {
							loader.parser.objects[selectedObjectID].components[x] = light;
						}
					}
				}
			}
			if (loader.parser.objects[selectedObjectID].HasComponent<RigidbodyComponent>()) {
				if (ImGui::CollapsingHeader("Rigidbody")) {
					RigidbodyComponent rigidbody = loader.parser.objects[selectedObjectID].GetComponent<RigidbodyComponent>();
					static float mass;
					mass = rigidbody.mass;
					ImGui::Text("Mass :     ");
					ImGui::SameLine();
					ImGui::InputFloat("##Mass", &mass);
					rigidbody.mass = mass;


					char ModelPathbuffer[255]{};
					ImGui::Text("Collider :     ");
					ImGui::SameLine();
					ImGui::InputTextWithHint("##Collider", "TODO : Implement rigidbody/collider relation", ModelPathbuffer, sizeof(ModelPathbuffer));
				}
			}
			if (loader.parser.objects[selectedObjectID].HasComponent<SphereColliderComponent>()) {
				if (ImGui::CollapsingHeader("Sphere Collider")) {
					SphereColliderComponent sphere = loader.parser.objects[selectedObjectID].GetComponent<SphereColliderComponent>();
					DrawVec3Control("Center : ", sphere.center);
					ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 3.0f));
					static float radius;
					radius = sphere.radius;
					ImGui::Text("Radius : ");
					ImGui::SameLine();
					ImGui::InputFloat("##Radius", &radius);
					sphere.radius = radius;
				}
			}
			if (loader.parser.objects[selectedObjectID].HasComponent<PlaneColliderComponent>()) {
				if (ImGui::CollapsingHeader("Plane Collider")) {
					PlaneColliderComponent plane = loader.parser.objects[selectedObjectID].GetComponent<PlaneColliderComponent>();
					DrawVec3Control("Center : ", plane.normal);
					ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 3.0f));
					static float distance;
					distance = plane.distance;
					ImGui::Text("Distance : ");
					ImGui::SameLine();
					ImGui::InputFloat("##Distance", &distance);
					plane.distance = distance;
				}
			}
		}


		ImGui::End();
	}

	//Content Browser
	if (showContentBrowser) {
		ImGui::Begin(ICON_FA_FILE"  Content Browser", &showContentBrowser, (ImGuiWindowFlags_MenuBar));
		//Menu bar
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu(ICON_FA_FILTER"  Filters")) {
				ImGui::MenuItem(ICON_FA_FILTER "  Select filter(s) :", NULL, false, false);
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem(ICON_FA_SAVE "  Save All")) {
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Add +")) {
				if (ImGui::MenuItem(ICON_FA_CLAPPERBOARD"  Scene")) {}
				if (ImGui::MenuItem(ICON_FA_FOLDER_PLUS "  Folder")) {}
				if (ImGui::BeginMenu(ICON_FA_CUBES "  Object")) {
					if (ImGui::MenuItem(ICON_FA_LIGHTBULB "  Light")) {}
					if (ImGui::MenuItem(ICON_FA_CUBE "  Model")) {}
					if (ImGui::MenuItem(ICON_FA_TINT "  Material")) {}
					if (ImGui::MenuItem(ICON_FA_CRYSTALBALL "  Shader")) {}
					if (ImGui::MenuItem(ICON_FA_CODE "  Script")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		static float padding = 37.0f;
		static float thumbnailSize = 60.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8, 0.8, 0.8, 0.1));
		ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 8);

		if (std::strlen(current_dir) > std::strlen(project_dir)) {

			ImGui::PushFont(solidBig);
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5, 0.7));
			ImGui::Button(ICON_FA_FOLDER_UPLOAD, ImVec2(thumbnailSize, thumbnailSize));
			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
				current_dir = extractPath(current_dir);
				changed_dir = true;
			}
			ImGui::PopStyleVar();

			ImGui::TextWrapped("..", thumbnailSize + 5);
			ImGui::PopFont();
			ImGui::NextColumn();

		}

		for (auto& file : manager.files) {
			if (auto folder_ = std::get_if<Folder>(&file)) {

				ImGui::PushID(folder_->name);

				ImGui::PushFont(solidBig);
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5, 0.7));
				ImGui::Button(ICON_FA_FOLDER, ImVec2(thumbnailSize, thumbnailSize));
				if (ImGui::IsItemClicked) {

				}
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0)) {
					current_dir = folder_->filepath;
					changed_dir = true;
				}
				ImGui::PopStyleVar();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX());
				ImGui::TextWrapped(folder_->name, thumbnailSize + 5);
				ImGui::PopFont();

				ImGui::NextColumn();

				ImGui::PopID();
			}
			if (auto file_ = std::get_if<File>(&file)) {
				ImGui::PushID(file_->name);

				ImGui::PushFont(solidBig);
				ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5, 0.7));
				const char* ICON = ICON_FA_FILE;
				switch (file_->type) {
				case LEVEL:
					ICON = ICON_FA_FILE_IMAGE;
					break;
				case TEXTURE:
					ICON = ICON_FA_TEXTURE_FILE;
					break;
				case TEXT:
					ICON = ICON_FA_FILE_ALT;
					break;
				case SHADER:
					ICON = ICON_FA_SHADER_FILE;
					break;
				case MATERIAL:
					ICON = ICON_FA_MATERIAL_FILE;
					break;
				case CPP:
					ICON = ICON_FA_CPP_FILE;
					break;
				case H:
					ICON = ICON_FA_H_FILE;
					break;
				case CODE:
					ICON = ICON_FA_CODE_FILE;
					break;
				}
				if (ImGui::Button(ICON, ImVec2(thumbnailSize, thumbnailSize))) {
					current_file = file_->id;
				}

				ImGui::PopStyleVar();

				ImGui::SetCursorPosX(ImGui::GetCursorPosX());
				std::string label = file_->name;
				label = label.append(file_->extension);

				ImGui::TextWrapped(label.c_str(), thumbnailSize + 5);
				ImGui::PopFont();


				ImGui::NextColumn();

				ImGui::PopID();
			}
		}

		if (changed_dir) {
			manager.files = ListFiles((char*)current_dir);
			changed_dir = false;
		}

		ImGui::PopStyleVar();
		ImGui::PopStyleColor();

		ImGui::End();
	}

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}