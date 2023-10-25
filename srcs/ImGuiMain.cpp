  #include "ImGuiMain.h"

ImGuiMain::ImGuiMain()
{
}

void ImGuiMain::Load(GLFWwindow* window, ImGuiIO& io)
{
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
	windowFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;
	io.ConfigFlags |= (ImGuiConfigFlags_NoMouseCursorChange, ImGuiConfigFlags_DockingEnable);
	io.Fonts->AddFontFromFileTTF("OpenSans-Medium.ttf", 13);
}

void ImGuiMain::Draw(GLFWwindow* window, Camera& cam, SceneLoader& loader, int& selectedObjectID, ImGuiIO& io)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::BeginMenu("New")) {
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
				ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/icons/objects/folder.png"), ImVec2(16, 16));
				ImGui::SameLine();
				if (ImGui::MenuItem("Project")) {}
				ImGui::EndMenu();
			}
			ImGui::Separator();
			if (ImGui::BeginMenu("Import to project")) {
				if (ImGui::MenuItem("Scene")) {
					std::string file = OpenWindowsFileDialog(L"Scene file (.json)\0*.json\0");
					if (file != "") {
						file = replaceCharacters(file, '\\', '/');
						file = file.substr(51, file.size() - 51);
						loader.LoadNewScene(file.c_str());
					}
				}
				if (ImGui::MenuItem("Model")) {
				}
				if (ImGui::MenuItem("Texture")) {
				}
				ImGui::EndMenu();
			}
			ImGui::Spacing();
			ImGui::Spacing();
			if (ImGui::BeginMenu("Save")) {
				if (ImGui::MenuItem("Scene")) {
				}
				if (ImGui::MenuItem("Object")) {
				}
				ImGui::EndMenu();
			}
			ImGui::Spacing();
			ImGui::Spacing();
			if (ImGui::BeginMenu("Open")) {
				if (ImGui::MenuItem("Project")) {}
				ImGui::EndMenu();
			}
			ImGui::Spacing();
			ImGui::Spacing();
			if (ImGui::MenuItem("Quit")) {}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit")) {
			if (ImGui::MenuItem("Select all")) {}
			if (ImGui::MenuItem("Redo")) {}
			if (ImGui::MenuItem("Undo")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Cut")) {}
			if (ImGui::MenuItem("Copy")) {}
			if (ImGui::MenuItem("Paste")) {}
			if (ImGui::MenuItem("Duplicate")) {}
			if (ImGui::MenuItem("Delete")) {}
			ImGui::Separator();
			if (ImGui::MenuItem("Editor Preferences")) {}
			if (ImGui::MenuItem("Project Settings")) {}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Window")) {
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
			ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/question.png"), ImVec2(16, 16));
			ImGui::SameLine();
			if (ImGui::MenuItem("Wiki")) {
				system("start https://github.com/Windokk/CrabEngine/wiki");
			}
			ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/code.png"), ImVec2(16, 16));
			ImGui::SameLine();
			if (ImGui::MenuItem("Engine Source code")) {
				system("start https://github.com/Windokk/CrabEngine");
			}
			ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/book.png"), ImVec2(16, 16));
			ImGui::SameLine(); // Move to the same line as the image
			if (ImGui::MenuItem("Engine Documentation"))
			{
				system("start https://github.com/Windokk/CrabEngine/blob/master/docs/README.md");
			}

			ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/list.png"), ImVec2(16, 16));
			ImGui::SameLine();
			if (ImGui::MenuItem("Credits")) {
				ImGui::Begin("Credits", nullptr, ImGuiWindowFlags_Popup);
				ImGui::End();
			}
			ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/help/certificate.png"), ImVec2(16, 16));
			ImGui::SameLine();
			if (ImGui::MenuItem("License")) {
			}
			ImGui::EndMenu();
		}
		// Add more menus here
		ImGui::EndMainMenuBar();
	}

	//Im Gui Viewport
	ImGui::SetNextWindowPos(ImVec2(320, 18));
	ImGui::SetNextWindowSize(ImVec2(640, 395));
	ImGui::Begin("Viewport", nullptr, windowFlags);
	ImGui::Image((void*)(intptr_t)loader.postProcessingTexture, ImVec2(640, 360), { 0,1 }, { 1,0 });
	isHoverViewport = (ImGui::IsItemHovered() || ImGui::IsWindowHovered()) && (io.MouseDown[GLFW_MOUSE_BUTTON_LEFT]);
	ImGui::End();

	//Outliner
	ImGui::Begin("Outliner", &showCloseButton);
	ImGui::BeginListBox("##", ImVec2(200, 80));
	for (int i = 0; i < loader.parser.objects.size(); i++) {
		if (ImGui::Selectable((loader.parser.objects[i].name).c_str())) {
			selectedObjectID = i;
			std::cout << selectedObjectID;
		}
	}
	ImGui::EndListBox();
	ImGui::End();

	//Details
	ImGui::Begin("Details", &showCloseButton);
	ImGui::End();

	//Content Browser
	ImGui::Begin("Content Browser", &showCloseButton, (ImGuiWindowFlags_MenuBar));
	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("Filters")) {
			ImGui::Image((void*)(intptr_t)LoadImageTexture("assets/defaults/gui/engine/menus/contentbrowser/filter.png"), ImVec2(16, 16));
			ImGui::SameLine();
			ImGui::MenuItem(("Select filter(s) :"), NULL, false, false);
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

	ImGui::EndFrame();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (isHoverViewport) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
		// Fetches the coordinates of the cursor
		cam.Inputs(window);
	}
	if (!isHoverViewport) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange, ImGuiConfigFlags_DockingEnable;
	}
}