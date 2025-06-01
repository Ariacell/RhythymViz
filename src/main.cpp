#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include "AudioPlayer.h" 

class VulkanApp {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanup();
    }

private:
    GLFWwindow* window;
    VkInstance instance;
    AudioPlayer audioPlayer;

    void initWindow() {
      printf("%s\n", glfwGetVersionString());
    //   glfwInitHint(GLFW::GLFW_PLATFORM, GLFW::GLFW_PLATFORM_NULL);  
      if(!glfwInit()) {
          throw std::runtime_error("Failed to init GLFW");
        }
        //glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        window = glfwCreateWindow(800, 600, "Vulkan Audio App", nullptr, nullptr);
        if (!window) {
            throw std::runtime_error("Failed to create GLFW window.");
        }
    }

    void initVulkan() {
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan Audio App";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("Failed to create Vulkan instance.");
        }
    }

    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            
            // Play an audio file when the user presses 'P'
            if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
                audioPlayer.playAudio("sample1.wav");
            }
        }
    }

    void cleanup() {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

// this function will be called internally by GLFW whenever an error occur.
void error_callback(int error, const char* description) {
    fprintf(stderr, "Error: %s (%d)\n", description, error);
}

int main() {
    VulkanApp app;

    glfwSetErrorCallback(error_callback);
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

