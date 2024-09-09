
#include "../include/scop.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
std::string getShader(std::string path);


const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

vec3 cameraPos   = vec3(0.0f, 0.0f,  3.0f);
vec3 cameraFront = vec3(0.0f, 0.0f, -1.0f);
vec3 cameraUp    = vec3(0.0f, 1.0f,  0.0f);
float pitch = 0;
float yaw = -90;

Camera camera(45, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1, 100.0f);

float lastX = SCR_WIDTH / 2;
float lastY = SCR_HEIGHT / 2;

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

vec3 lightPos(1.2f, 1.0f, 2.0f);

int main(void)
{

    if (glfwInit() == GLFW_FALSE)
        return 1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OGL", NULL, NULL);

    if (window == NULL){
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }  

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.2, 0.2, 0.2, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback); 

    Shader lightShader("shaders/light/light.vs", "shaders/light/light.fs");
    Shader unlitShader("shaders/unlit/unlit.vs", "shaders/unlit/unlit.fs");

    //*---------------------------



    float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
    //*Vertex things objects
    
    unsigned int lightVAO;
    unsigned int VBO;

    glGenVertexArrays(1, &lightVAO); 
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(lightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    //* Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    
    lightShader.use();
    lightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    lightShader.setVec3("ambientLightColor",  0.2, 0.2, 0.2);
    lightShader.setVec3("lightColor",  1.0f, 1.0f, 1.0f);
    lightShader.setVec3("lightPos", lightPos);
    //* Render loop

    
    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, [MODE]); // GL_LINE = Wireframe ; GL_FILL = Fill
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //Vsync (not working in dual monitor + wsl set up)
    
    /* lightShader.setVec3("objectColor", 0.8f, 0.4f, 0.7f);
    lightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f); */

    while(!glfwWindowShouldClose(window)){
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        processInput(window);
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        lightShader.use();

        camera.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        mat4 projection = camera.getPerspectiveProjection();
        mat4 view = camera.getViewMatrix();
        mat4 model(1);
        
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setMat4("model", model);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        unlitShader.use();

        model = mat4(1);
        model.scale(0.4, 0.4, 0.4);
        model.translate(lightPos);

        unlitShader.setMat4("projection", projection);
        unlitShader.setMat4("view", view);
        unlitShader.setMat4("model", model);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // int modelLoc = glGetUniformLocation(shader.ID, "model");
        // glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model.value_ptr());
        // int viewLoc = glGetUniformLocation(shader.ID, "view");
        // glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view.value_ptr());
        // int projectionLoc = glGetUniformLocation(shader.ID, "projection");
        // glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.value_ptr());

        
        // glBindVertexArray(lightVAO); // seeing as we only have a single VAO there's no need to bind it every time
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0); // no need to unbind it every time 
        

        glfwSwapBuffers(window);
        glfwPollEvents();

        /* GLenum error = glGetError();
        if (error != GL_NO_ERROR){
            const char* errorString = nullptr;
            switch (error) {
                case GL_INVALID_ENUM:      errorString = "GL_INVALID_ENUM"; break;
                case GL_INVALID_VALUE:     errorString = "GL_INVALID_VALUE"; break;
                case GL_INVALID_OPERATION: errorString = "GL_INVALID_OPERATION"; break;
                case GL_STACK_OVERFLOW:    errorString = "GL_STACK_OVERFLOW"; break;
                case GL_STACK_UNDERFLOW:   errorString = "GL_STACK_UNDERFLOW"; break;
                case GL_OUT_OF_MEMORY:     errorString = "GL_OUT_OF_MEMORY"; break;
                case GL_INVALID_FRAMEBUFFER_OPERATION: errorString = "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
                default:                   errorString = "Unknown error"; break;
            }

            std::cerr << errorString;
            glfwSetWindowShouldClose(window, true);
        } */
    }



    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}

void calRot(const float& pitchAdd, const float& yawAdd);
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f;
    /* vec3 fpsDirection;
    fpsDirection[0] = cos(DEG_TO_RAD(yaw));
    fpsDirection[2] = sin(DEG_TO_RAD(yaw));
    fpsDirection = fpsDirection.normalized(); */
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos = cameraPos + cameraFront * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos = cameraPos - cameraFront * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos = cameraPos - cross(cameraFront, cameraUp).normalized() * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos = cameraPos + cross(cameraFront, cameraUp).normalized() * cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos = cameraPos + vec3(0, 1, 0) * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos = cameraPos - vec3(0, 1, 0) * cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        calRot(1, 0);
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        calRot(-1, 0);
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        calRot(0, 1);
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        calRot(0, -1);
}

void calRot(const float& pitchAdd, const float& yawAdd){

    const float sensitivity = 0.04;
    yaw += yawAdd * sensitivity;
    pitch += pitchAdd * sensitivity;
    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

        
    vec3 direction;
    direction[0] = cos(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));
    direction[1] = sin(DEG_TO_RAD(pitch));
    direction[2] = sin(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));

    cameraFront = direction.normalized();
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    
    if (firstMouse){
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    const float sensitivity = 0.001f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

        
    vec3 direction;
    direction[0] = cos(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));
    direction[1] = sin(DEG_TO_RAD(pitch));
    direction[2] = sin(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));

    cameraFront = direction.normalized();
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    float fov = camera.getFov() - (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;

    camera.setFov(fov);
}