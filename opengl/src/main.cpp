#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>


//compiling shader
static unsigned int CompiledShader( unsigned int type, const std::string& source) {
    /*type = vertex shader of fragment shader
      soucre = shader string or code */
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE) {

        std::cout << "Shader didnt compile properly";

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);

        return 0;
    }

    return id;
}

//creating shader
static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    /*vertexShader or fragment shader code in the form of string*/

    unsigned int program = glCreateProgram();                                       //
    unsigned int vs = CompiledShader(GL_VERTEX_SHADER, vertexShader);               //vertex shader creation
    unsigned int fs = CompiledShader(GL_FRAGMENT_SHADER, fragmentShader);           //fragment shader creation

    glAttachShader(program, vs);                                                    //attaching vertex shader to program
    glAttachShader(program, fs);                                                    //attaching fragment shader to program
    glLinkProgram(program);                                                         //
    glValidateProgram(program);                                                     //

    glDeleteShader(vs);                                                             //deleting and freeing vertex shader
    glDeleteShader(fs);                                                             //deleting and freeing fragment shader

    return program;

}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;



    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Window", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        std::cout << "GLEW not ok" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

    //modern opengl creating triangle
    float position[6] = {                                                               //data
        -0.5,-0.5f,                                                                     //vertex 1 x,y
        0.0f,0.5f,                                                                      //vertex 2 x,y
        0.5f,-0.5f,                                                                     //vertex 3 x,y
    };                                                                                  
    unsigned int buffer;                                                                //creating location/buffer
    glGenBuffers(1, &buffer);                                                           //gives id
    glBindBuffer(GL_ARRAY_BUFFER, buffer);                                              //binding id to buffer
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), position, GL_STATIC_DRAW);         //adding/copying data and size 
    glEnableVertexAttribArray(0);                                                       //enabling generic vertex
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);              //adding attribute of vertex
    
    std::string vertexShader = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        " gl_Position = position;\n "
        "}\n";

    std::string fragmentShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        " color = vec4( 1.0, 0.0, 0.0, 1.0);"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //draws triangle(array of data defined "position")
        glDrawArrays(GL_TRIANGLES, 0, 3);                                               

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }
    glDeleteProgram(shader);
    glfwTerminate();
    return 0;
}