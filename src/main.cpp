
#include "../include/scop.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
std::string getShader(std::string path);
void textureTransition(Shader& shader);



float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float textureTransitionValue = 0;
bool is_texture_active = false;

Camera camera(45, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1, 100.0f);

//int load_project(int argc, char** argv){
int main(int argc, char** argv){

    if (argc < 2){
        std::cerr << "Need more arguments" << std::endl;
        return 1;
    }

    //* Window management
    GLFWwindow* window = createWindow(SCR_WIDTH, SCR_HEIGHT, "OGL");
    if (window == NULL)
        return 1;

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    
    glClearColor(0.1, 0.1, 0.1, 1.0f);


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


    //* Lights

    vec3 ambient_light_color(0.05f, 0.05f, 0.05f);

    point_light light;
    light.position = vec3(1.2f, 2.0f, -1.0f);
    light.color = vec3(1, 1, 1);
    light.diffuse = vec3(0.5f, 0.5f, 0.5f);
    light.constant = 1;
    light.linear = 0.09f;
    light.quadratic = 0.032f;

    Object light_ob("resources/Cubo.obj", material_unlit);
    light_ob.setPosition(light.position);
    light_ob.scale(vec3(0.2, 0.2, 0.2));
    obs.push_back(&light_ob);


    //* Uniform buffers objects

    unsigned int UBO_matrices = createUBO(2 * MAT4_SIZE);
    unsigned int UBO_lights = createUBO(4 * VEC3_SIZE + sizeof(float) * 3);

    GLuint matrices_binding_point = 0;
    GLuint lights_binding_point = 1;

    glBindBufferBase(GL_UNIFORM_BUFFER, matrices_binding_point, UBO_matrices);
    glBindBufferBase(GL_UNIFORM_BUFFER, lights_binding_point, UBO_lights);

    uniformBlockBind(shader_lit.ID, "Matrices", matrices_binding_point);
    uniformBlockBind(shader_unlit.ID, "Matrices", matrices_binding_point);
    uniformBlockBind(shader_lit.ID, "Lights", lights_binding_point);
    
    setLightBufferValues(UBO_lights, ambient_light_color, light);

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

        processInput(window, camera, deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

        light_ob.setColor(light.color);

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


void mouse_callback(GLFWwindow* window, double xpos, double ypos){
    static float lastX = SCR_WIDTH / 2;
    static float lastY = SCR_HEIGHT / 2;
    static bool firstMouse = true;
    static float pitch = 0;
    static float yaw = -90;

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

    yaw += xoffset * CAMERA_SENSIBILITY;
    pitch += yoffset * CAMERA_SENSIBILITY;

    if (pitch > 89.0f)
        pitch = 89.0f;
    else if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction;
    direction[0] = cos(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));
    direction[1] = sin(DEG_TO_RAD(pitch));
    direction[2] = sin(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));

    camera.setFront(direction.normalized());
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