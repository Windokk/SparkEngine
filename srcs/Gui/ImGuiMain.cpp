#include "ImGuiMain.h"


ImGuiMain::ImGuiMain()
{
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
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_16_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	solid = io.Fonts->AddFontFromFileTTF("assets/defaults/gui/engine/fonts/fa-solid-900.ttf", 13.0f,&icons_config, icons_ranges);
	ImGuiMain::SetupImGuiStyle();
}

void ImGuiMain::Draw(GLFWwindow* window, Camera& cam, SceneLoader& loader, int& selectedObjectID, ImGuiIO& io)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();


	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

	//Teleports to selectedObject when pressing F
	if (io.KeysDown[GLFW_KEY_F]) {
		cam.Position = loader.objects_Transforms[selectedObjectID].Location;
	}

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem(ICON_FA_FILE_PLUS "  New", "Ctrl+N")) {
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
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Build")) {
			if (ImGui::MenuItem("Package Project")) {}
			if (ImGui::MenuItem("Package Game")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem(ICON_FA_QUESTION"  Wiki")) {
				system("start https://github.com/Windokk/CrabEngine/wiki");
			}
			if (ImGui::MenuItem(ICON_FA_CODE"  Engine Source code")) { system("start https://github.com/Windokk/CrabEngine"); }
			if (ImGui::MenuItem(ICON_FA_BOOK"  Engine Documentation"))
			{
				system("start https://github.com/Windokk/CrabEngine/blob/master/docs/README.md");
			}
			if (ImGui::MenuItem(ICON_FA_LIST"  Credits")) {
				showCreditsWindow = true;
			}
			if (ImGui::MenuItem(ICON_FA_CERTIFICATE"  License")) {
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}

	if (showCreditsWindow) {
		ImGui::Begin("Credits", &showCreditsWindow);
		ImGui::End();
	}

	static ImGuizmo::MODE currentGizmoMode(ImGuizmo::WORLD);
	static ImGuizmo::OPERATION currentGizmoOperation(ImGuizmo::TRANSLATE);

	static bool useSnap = false;
	static float snap[3] = { 1.f, 1.f, 1.f };
	static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
	static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
	static bool boundSizing = false;
	static bool boundSizingSnap = false;

	float viewManipulateRight = io.DisplaySize.x;
	float viewManipulateTop = 0;

	ImGuiStyle& style = ImGui::GetStyle();

	//Viewport
	if (showViewport) {
		style.WindowPadding = ImVec2(0, 0);
		ImGui::Begin(ICON_FA_CAMERA"  Viewport", &showViewport);

		//Viewport variables
		viewportPos = ImGui::GetWindowPos();
		viewportSize = ImGui::GetWindowSize();

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5);
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

		ImGuizmo::SetDrawlist();
		ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y);
		viewManipulateRight = ImGui::GetWindowPos().x + ImGui::GetContentRegionAvail().x;
		viewManipulateTop = ImGui::GetWindowPos().y;

		const float* cameraView = glm::value_ptr(cam.view);
		const float* cameraProjection = glm::value_ptr(cam.projection);

		ImGui::Image((void*)(intptr_t)loader.framebufferTexture, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));
		isHoverViewport = (ImGui::IsItemHovered());

		if (isHoverViewport && ImGui::IsWindowDocked() && !ImGuizmo::IsUsingAny()) {

			cam.Inputs(window, 0.01F, 100.0f, ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x / 2, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y / 2), ImGui::GetWindowSize());
		}
		if (!isHoverViewport) {

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


		ImGui::End();
		style.WindowPadding = ImVec2(8, 8);

		if (io.KeysDown[GLFW_KEY_T]) {
			currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
		}

		if (io.KeysDown[GLFW_KEY_R]) {
			currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
		}

		if (io.KeysDown[GLFW_KEY_S]) {
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
		strcpy(buffer, loader.parser.objects[selectedObjectID].name.c_str());
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
					glm::vec3 axis = glm::axis(transform.Rotation);
					float angle = glm::angle(transform.Rotation);
					// Modify the axis and angle values
					static glm::vec3 axisAngle = axis * angle;
					DrawVec3Control("Rotation", axisAngle);
					ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX(), ImGui::GetCursorPosY() + 3.0f));
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
					///////////   SCALE   ////////////////////////
					//////////////////////////////////////////////
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
			ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.8f, 0.2f, 0.2f, 1.0f));
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
			ImGui::PopStyleColor();
			ImGui::EndMenuBar();
		}
		ImGui::End();
	}

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}