
#include "../include/scop.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
std::string getShader(std::string path);
void textureTransition(Shader& shader);
bool is_texture_active = false;

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


float textureTransitionValue = 0;

//int load_project(int argc, char** argv){
int main(int argc, char** argv){

    if (argc < 2){
        std::cerr << "Need more arguments" << std::endl;
        return 1;
    }

    //* Window management

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
        return 1;
    }  

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glClearColor(0.1, 0.1, 0.1, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    //* Shaders load

    Shader shader_lit("./shaders/lit/lit.vs", "./shaders/lit/lit.fs");
    Shader shader_unlit("./shaders/unlit/unlit.vs", "./shaders/unlit/unlit.fs");


    //* Materials

    Material material_lit(shader_lit, vec3(1, 0.5, 1), "textures/checkerColors.jpg");
    Material material_unlit(shader_unlit, vec3(1, 1, 1), "");


    //* Objects load

    std::vector<Object*> obs;

    Object ob1(argv[1], material_lit);
    ob1.move(vec3(0, 0, -5));
    obs.push_back(&ob1);

    ob1.rotate(vec3(0, -90, 0));
    ob1.scale(vec3(3, 3, 3));

    //* Objects customization




    //* Lights

    vec3 ambient_light_color(0.05f, 0.05f, 0.05f);
    vec3 light_pos(1.2f, 2.0f, -1.0f);
    vec3 light_color(1, 1, 1);
    vec3 light_diffuse(0.5f, 0.5f, 0.5f);
    float light_constant = 1;
    float light_linear = 0.09f;
    float light_quadratic = 0.032f;

    Object light_ob("resources/Cubo.obj", material_unlit);
    light_ob.setPosition(light_pos);
    light_ob.scale(vec3(0.2, 0.2, 0.2));
    obs.push_back(&light_ob);


    //* Uniform buffers objects

    unsigned int UBO_matrices;
    glGenBuffers(1, &UBO_matrices);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO_matrices);
    glBufferData(GL_UNIFORM_BUFFER, 2 * MAT4_SIZE, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    unsigned int UBO_lights;
    glGenBuffers(1, &UBO_lights);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO_lights);
    glBufferData(GL_UNIFORM_BUFFER, 4 * VEC3_SIZE + sizeof(float) * 3, NULL, GL_STATIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    GLuint matrices_binding_point = 0;
    GLuint lights_binding_point = 1;

    unsigned int uniform_block_index_lit_matrices = glGetUniformBlockIndex(shader_lit.ID, "Matrices");
    unsigned int uniform_block_index_unlit_matrices = glGetUniformBlockIndex(shader_unlit.ID, "Matrices");
    unsigned int uniform_block_index_lit_lights = glGetUniformBlockIndex(shader_lit.ID, "Lights");

    glUniformBlockBinding(shader_lit.ID, uniform_block_index_lit_matrices, matrices_binding_point);
    glUniformBlockBinding(shader_unlit.ID, uniform_block_index_unlit_matrices, matrices_binding_point);
    glUniformBlockBinding(shader_lit.ID, uniform_block_index_lit_lights, lights_binding_point);

    glBindBufferBase(GL_UNIFORM_BUFFER, matrices_binding_point, UBO_matrices);
    glBindBufferBase(GL_UNIFORM_BUFFER, lights_binding_point, UBO_lights);
    
    glBindBuffer(GL_UNIFORM_BUFFER, UBO_lights);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, VEC3_SIZE, ambient_light_color.value_ptr());
    glBufferSubData(GL_UNIFORM_BUFFER, 1 * VEC3_SIZE, VEC3_SIZE, light_color.value_ptr());
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * VEC3_SIZE, VEC3_SIZE, light_pos.value_ptr());
    glBufferSubData(GL_UNIFORM_BUFFER, 3 * VEC3_SIZE, VEC3_SIZE, light_diffuse.value_ptr());
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * VEC3_SIZE, sizeof(float), &light_constant);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * VEC3_SIZE + sizeof(float), sizeof(float), &light_linear);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * VEC3_SIZE + sizeof(float) * 2, sizeof(float), &light_quadratic);


    //* Render settigns

    glEnable(GL_DEPTH_TEST);
    // glPolygonMode(GL_FRONT_AND_BACK, [MODE]); // GL_LINE = Wireframe ; GL_FILL = Fill
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); //Vsync (not working in dual monitor + wsl set up)



    //* Render loop

    while(!glfwWindowShouldClose(window)){
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        const mat4& projection = camera.getPerspectiveProjection();
        const mat4& view = camera.getViewMatrix();

        /* light_pos[0] = cos(currentFrame) * 4;
        light_pos[1] = cos(currentFrame * 2.16);
        light_pos[2] = sin(currentFrame) * 4;
        light_ob.setPosition(light_pos); */

        /* light_color[0] = cos(currentFrame * 4.234) / 2 + 0.5;
        light_color[1] = sin(currentFrame * 4.234) / 2 + 0.5;*/
        
        /* glBindBuffer(GL_UNIFORM_BUFFER, UBO_lights);
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * VEC3_SIZE, VEC3_SIZE, light_pos.value_ptr());
        glBufferSubData(GL_UNIFORM_BUFFER, VEC3_SIZE, VEC3_SIZE, light_color.value_ptr()); */

        glBindBuffer(GL_UNIFORM_BUFFER, UBO_matrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, MAT4_SIZE, projection.value_ptr());
        glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE, MAT4_SIZE, view.value_ptr());
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        light_ob.setColor(light_color);

        textureTransition(shader_lit);
        ob1.setRotation(vec3(sin(currentFrame) * 10, 20 * currentFrame, 0));
        for (unsigned int i = 0; i < obs.size(); i++)
            obs[i]->render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    if (window)
        glViewport(0, 0, width, height);
}

void calRot(const float& pitchAdd, const float& yawAdd);
void processInput(GLFWwindow* window){
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float cameraSpeed = 2.5f;
    vec3 fpsDirection;
    fpsDirection[0] = cos(DEG_TO_RAD(yaw));
    fpsDirection[2] = sin(DEG_TO_RAD(yaw));
    fpsDirection = fpsDirection.normalized();
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos = cameraPos + fpsDirection * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos = cameraPos - fpsDirection * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos = cameraPos - cross(cameraFront, cameraUp).normalized() * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos = cameraPos + cross(cameraFront, cameraUp).normalized() * cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        cameraPos = cameraPos + vec3(0, 1, 0) * cameraSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        cameraPos = cameraPos - vec3(0, 1, 0) * cameraSpeed * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        if (textureTransitionValue == 0 || textureTransitionValue == 1)
            is_texture_active = !is_texture_active;
    }

}

void calRot(const float& pitchAdd, const float& yawAdd){

    const float sensitivity = 0.2;
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

void textureTransition(Shader& shader){
    if (is_texture_active && textureTransitionValue < 1){
        textureTransitionValue += deltaTime;
        if (textureTransitionValue > 1)
            textureTransitionValue = 1;            
        shader.use();
        shader.setFloat("textureTransitionValue", textureTransitionValue);
    }
    else if (!is_texture_active && textureTransitionValue > 0){
        textureTransitionValue -= deltaTime;
        if (textureTransitionValue < 0)
            textureTransitionValue = 0;
        shader.use();
        shader.setFloat("textureTransitionValue", textureTransitionValue);
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    
    if (!window)
        return;
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
    if (!window)
        return;

    float fov = camera.getFov() - (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 90.0f)
        fov = 90.0f;

    camera.setFov(fov);

    if (xoffset)
        return;    
}
