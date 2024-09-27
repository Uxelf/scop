
#include "../include/scop.hpp"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
std::string getShader(std::string path);
void lightMovement(float limit, Object& light_object, unsigned int UBO_lights);
void textureTransition(Shader& shader);



float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

float textureTransitionValue = 0;
bool is_texture_active = false;
bool is_light_moving = true;

Camera camera(45, (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1, 100.0f);

//int load_project(int argc, char** argv){
int main(int argc, char** argv){

    if (argc < 2){
        std::cerr << "Need more arguments" << std::endl;
        return 1;
    }

    print_instructions();
    
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

    std::vector<Object*> scene_objects;

    for (int i = 0; i < argc - 1; i++){
        scene_objects.push_back(new Object(argv[i + 1], material_lit));
        scene_objects[i]->move(vec3(OBJECTS_SEPARATION * i, 0, 0));
        scene_objects[i]->scale(vec3(1.2, 1.2, 1.2));
    }


    //* Lights

    vec3 ambient_light_color(0.05f, 0.05f, 0.05f);

    point_light light;
    light.position = vec3(0.0f, 1.0f, 3.3f);
    light.color = vec3(1, 1, 1);
    light.diffuse = vec3(0.5f, 0.5f, 0.5f);
    light.constant = 1;
    light.linear = 0.09f;
    light.quadratic = 0.032f;

    Object light_ob = Object("resources/Cubo.obj", material_unlit);
    scene_objects.push_back(&light_ob);
    light_ob.setPosition(light.position);
    light_ob.scale(vec3(0.2, 0.2, 0.2));

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

    Object xO = Object("resources/Cubo.obj", material_unlit);
    xO.scale(vec3(1, 0.2, 0.2));
    xO.move(vec3(0.75, 0, 0));
    Object yO = Object("resources/Cubo.obj", material_unlit);
    yO.scale(vec3(0.2, 1, 0.2));
    yO.move(vec3(0, 0.75, 0));
    Object zO = Object("resources/Cubo.obj", material_unlit);
    zO.scale(vec3(0.2, 0.2, 1));
    zO.move(vec3(0, 0, 0.75));

    //* Render loop
    camera.move(vec3(0.5, 1, 1.5));

    while(!glfwWindowShouldClose(window)){
        
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;  

        processInput(window, camera, deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const mat4& projection = camera.getPerspectiveProjection();
        const mat4& view = camera.getViewMatrix();

        glBindBuffer(GL_UNIFORM_BUFFER, UBO_matrices);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, MAT4_SIZE, projection.value_ptr());
        glBufferSubData(GL_UNIFORM_BUFFER, MAT4_SIZE, MAT4_SIZE, view.value_ptr());
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        light_ob.setColor(light.color);

        textureTransition(shader_lit, deltaTime);
        lightMovement((argc - 2) * OBJECTS_SEPARATION, light_ob, UBO_lights);
        for (unsigned int i = 0; i < scene_objects.size(); i++){
            if (i != scene_objects.size() - 1)
                scene_objects[i]->rotate(vec3(0, deltaTime * 4, 0));
            scene_objects[i]->render();
        }

        xO.render();
        yO.render();
        zO.render();
        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    for (unsigned i = 0; i < scene_objects.size() - 1; i++)
        delete scene_objects[i];
    glfwTerminate();

    return 0;
}

void lightMovement(float limit, Object& light_object, unsigned int UBO_lights){
    static bool moving_right = true;
    int x_dir = (moving_right)? 1 : -1;
    if (limit != 0 && is_light_moving){
        light_object.move(vec3(x_dir * deltaTime * 4, 0, 0));
        if (moving_right && light_object.getPosition()[0] >= limit)
            moving_right = false;
        else if (!moving_right && light_object.getPosition()[0] <= 0)
            moving_right = true;

        glBindBuffer(GL_UNIFORM_BUFFER, UBO_lights);
        glBufferSubData(GL_UNIFORM_BUFFER, 2 * VEC3_SIZE, VEC3_SIZE, light_object.getPosition().value_ptr());
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
    direction[0] = -cos(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));
    direction[1] = sin(DEG_TO_RAD(pitch));
    direction[2] = -sin(DEG_TO_RAD(yaw)) * cos(DEG_TO_RAD(pitch));

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