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

void LoadSceneFromFile(SceneLoader& loader) {
	std::string file = OpenWindowsFileDialog(L"Scene file (.json)\0*.json\0");
	if (file != "") {
		file = replaceCharacters(file, '\\', '/');
		std::string file_relative;
		file_relative = make_relative(file, file_relative);
		std::cout << file_relative;
		loader.LoadNewScene(file.c_str());
	}
}

void ImGuiMain::Load(GLFWwindow* window, ImGuiIO& io)
{
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	io.ConfigFlags |= (ImGuiConfigFlags_NoMouseCursorChange, ImGuiConfigFlags_DockingEnable);
	io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/defaults/gui/engine/fonts/OpenSans-Medium.ttf", 13);
	ImGuiMain::SetupImGuiStyle();

	// Initialize notify
	ImGui::MergeIconsWithLatestFont();

	ImFontConfig font_cfg;
	font_cfg.FontDataOwnedByAtlas = false;
	io.Fonts->AddFontFromMemoryTTF((void*)tahoma, sizeof(tahoma), 17.f, &font_cfg);

	

	currentGizmoMode = ImGuizmo::WORLD;
	currentGizmoOperation = ImGuizmo::TRANSLATE;
}

void ImGuiMain::Draw(GLFWwindow* window, Camera& cam, SceneLoader& loader, int& selectedObjectID, ImGuiIO& io)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();

	ImGui::PushStyleColor(ImGuiCol_DockingPreview, ImVec4(0.06866955757141113f, 0.06866887211799622f, 0.06866887211799622f, 1.0f));


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
		cam.Position = loader.objects_Transforms[selectedObjectID].Location;
	}

	//Main Menu Bar
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem(ICON_FA_FILE_PLUS "  New", "Ctrl+N")) {
				showNewDialog = true;
			}
			ImGui::Separator();
			if (ImGui::BeginMenu(ICON_FA_FILE_UPLOAD "  Import to project")) {
				if (ImGui::MenuItem(ICON_FA_CLAPPERBOARD "  Scene")) {
					LoadSceneFromFile(loader);
				}
				if (ImGui::MenuItem(ICON_FA_CUBE "  Model")) {
				}
				if (ImGui::MenuItem(ICON_FA_IMAGE"  Texture")) {
				}
				ImGui::EndMenu();
			}

			if (ImGui::MenuItem(ICON_FA_SAVE "  Save", "Ctrl+S")) {
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
			if (ImGui::MenuItem("Project Settings")) {}

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
			if (ImGui::MenuItem("Find item","Ctrl+F")) {

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
			if (ImGui::MenuItem(ICON_FA_BOOK"  Engine Documentation")){
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
		ImGui::EndMainMenuBar();
	}
	
	//New file Popup
	if (showNewDialog) {
		ImGui::OpenPopup("Action Menu");
		if (ImGui::BeginPopupModal("Action Menu", &showNewDialog)) {
			TextCentered("Create a new file :  ");
			ImGui::Separator();
			if(io.KeysDown[GLFW_KEY_ENTER]) {
				showNewDialog = false;
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
		bool DevlinkSelected  = false;
		ImGui::Selectable(std::string("Documentation for Developpers").c_str(), &DevlinkSelected, ImGuiSelectableFlags_None, ImVec2(textWidth, 0));
		if(DevlinkSelected){
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
			std::string iconString = ICON_FA_CUBE + spacing + loader.parser.objects[i].name;
			if(ImGui::Selectable(iconString.c_str())) {
				selectedObjectID = i;
				
			}
			ImGui::Separator();
		}
		ImGui::End();
	}

	//Details
	if(showDetails){
		ImGui::Begin(ICON_FA_LIST"  Details",&showDetails);
		ImGui::Text("Name : ");
		ImGui::SameLine();
		static char buffer[256];
		strcpy_s(buffer, loader.parser.objects[selectedObjectID].name.c_str());
		style.FrameRounding = 6.0f;
		ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 5);
		ImGui::InputText("##ObjectName", buffer, 600);
		style.FrameRounding = 0.0f;
		ImGui::Separator();
		for (int a = 0; a < loader.parser.objects[selectedObjectID].components.size(); a++) {
			if (std::holds_alternative<TransformComponent>(loader.parser.objects[selectedObjectID].components[a])) {
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
			else if (std::holds_alternative<ModelComponent>(loader.parser.objects[selectedObjectID].components[a])) {
				if (ImGui::CollapsingHeader("Model")) {
					ModelComponent model = std::get<ModelComponent>(loader.parser.objects[selectedObjectID].components[a]);
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
			else if (std::holds_alternative<LightComponent>(loader.parser.objects[selectedObjectID].components[a])) {
				if (ImGui::CollapsingHeader("Light")) {
					ImGui::Separator();
					LightComponent light = std::get<LightComponent>(loader.parser.objects[selectedObjectID].components[a]);
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
		}

		ImGui::End();
	}

	//Content Browser
	if (showContentBrowser) {
		ImGui::Begin(ICON_FA_FILE"  Content Browser", &showContentBrowser, (ImGuiWindowFlags_MenuBar));
		if (ImGui::BeginMenuBar()) {
			if (ImGui::BeginMenu(ICON_FA_FILTER"  Filters")) {
				ImGui::MenuItem(ICON_FA_FILTER "  Select filter(s) :", NULL, false, false);
				ImGui::Separator();
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::MenuItem("Save All")) {
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Add +")) {
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/scene.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::MenuItem("Scene")) {}
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/cube.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::BeginMenu("Object")) {
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/light.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Light")) {}
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/cube.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Model")) {}
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/material.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Shader")) {}
					ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/script.png"), ImVec2(16, 16));
					ImGui::SameLine();
					if (ImGui::MenuItem("Script")) {}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	

	ImGui::PopStyleColor();

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}