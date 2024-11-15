#include <iostream>

#include <glad/glad.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <nfd.h>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

static nfdu8filteritem_t image_filters[1] = {{"Images code", "png,jpg,jpeg"}};
static nfdopendialogu8args_t image_open_args = {image_filters, 1};

static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

static std::string get_file_dialogue() {
	nfdu8char_t *outPath;
	nfdresult_t result = NFD_OpenDialogU8_With(&outPath, &image_open_args);

	if (result == NFD_OKAY) {

		std::string stringPath = std::string(outPath);
		NFD_FreePathU8(outPath);
		return stringPath;
	}

	std::cerr << "Failed to get file from dialogue" << std::endl;
	return "";
}

static void load_image(const std::string& path) {
	
}

static void load_image_from_dialogue() {
	const std::string image_path = get_file_dialogue(); 
	if (!image_path.empty()) {
		load_image(image_path);
	};
}

int main() {

	
	glfwSetErrorCallback(glfw_error_callback);
	constexpr ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW3" << std::endl;
		return -1;
	}


	GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World!", nullptr, nullptr);
	if (!window) {
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
			return;
		}

		if (key == GLFW_KEY_O && mods & (GLFW_MOD_CONTROL) && action == GLFW_PRESS) {
			load_image_from_dialogue();	
		}
	});

	// Setup Dear ImGui context
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void) io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
	

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	int display_w, display_h;

	bool showDemo = true;

	NFD_Init();
	

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
		{
			ImGui_ImplGlfw_Sleep(10);
			continue;
		}
		
		// Rendering

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glfwGetFramebufferSize(window, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		
		if (ImGui::BeginMainMenuBar()) {
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open", "Ctrl+O")) {

					load_image_from_dialogue();
				}
				if (ImGui::MenuItem("Save", "Ctrl+S")) {
				}
				ImGui::EndMenu();
			}
			ImGui::EndMainMenuBar();
		}
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(window);
	}

	NFD_Quit();

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();


	return 0;
}
