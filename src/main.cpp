#include <scop.hpp>


int main(int argc, char** argv){

    if (argc < 2){
        std::cerr << "Need more arguments" << std::endl;
        return 1;
    }

    print_instructions();

    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "OGL");
    if (window == NULL)
        return 1;
    
    loadScene(window, argc, argv);

    glfwTerminate();

    return 0;
}
