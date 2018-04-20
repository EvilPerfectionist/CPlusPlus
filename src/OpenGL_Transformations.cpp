#define GLEW_STATIC

#include <GL/glew.h>
#include <SOIL/SOIL.h>
#include <SFML/Window.hpp>
#include <chrono>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Shader sources
const GLchar* vertexSource = R"glsl(
    #version 450 core
    in vec2 position;
    in vec3 color;
    in vec2 texcoord;

    out vec3 Color;
    out vec2 Texcoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 proj;

    void main()
    {
        Color = color;
        Texcoord = texcoord;
        //gl_Position = vec4(position, 0.0, 1.0);
        gl_Position = proj * view * model * vec4(position, 0.0, 1.0);
    }
)glsl";
const GLchar* fragmentSource = R"glsl(
    #version 450 core
    //uniform vec3 triangleColor;
    in vec3 Color;
    in vec2 Texcoord;
    out vec4 outColor;
    uniform sampler2D texKitten;
    uniform sampler2D texPuppy;
    uniform float time;
    void main()
    {
        //outColor = vec4(triangleColor, 1.0);
        //outColor = vec4(Color, 1.0);
        vec4 colKitten = texture(texKitten, Texcoord);
        vec4 colPuppy = texture(texPuppy, Texcoord);
        float factor = (sin(time * 3.0) + 1.0) / 2.0;
        //outColor = mix(colKitten, colPuppy, 0.5);
        //outColor = mix(colKitten, colPuppy, factor);
        //outColor = texture(tex, Texcoord) * vec4(Color, 1.0);
        if (Texcoord.y < 0.5)
            outColor = texture(texKitten, Texcoord);
        else
            outColor = texture(texKitten,
                vec2(Texcoord.x + sin(Texcoord.y * 60.0 + time * 2.0) / 30.0, 1.0 - Texcoord.y)
            ) * vec4(0.7, 0.7, 1.0, 1.0);
    }
)glsl";

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 2; // Optional
    // Request OpenGL version 3.2 (optional but recommended)
    settings.majorVersion = 3;
    settings.minorVersion = 2;

    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Close, settings);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    // Create Vertex Array Object
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create a Vertex Buffer Object and copy the vertex data to it
    GLuint vbo;
    glGenBuffers(1, &vbo);

    GLfloat vertices[] = {
            //  Position      Color        Texcoords
            -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
            0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
            -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // Bottom-left
    };

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint ebo;
    glGenBuffers(1, &ebo);

    GLuint elements[] = {
            0, 1, 2,
            2, 3, 0
    };

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(elements), elements, GL_STATIC_DRAW);

    // Load texture
    GLuint textures[2];
    glGenTextures(2, textures);

    int width, height;
    unsigned char* image;

    // Black/white checkerboard
    //float pixels[] = {
    //        0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
    //        1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
    //};
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    image = SOIL_load_image("/home/leon/Instrument_Pose_Estimation/Test_C/images/sample.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    //float color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    image = SOIL_load_image("/home/leon/Instrument_Pose_Estimation/Test_C/images/sample2.png", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Create and compile the vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    // Create and compile the fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    // Link the vertex and fragment shader into a shader program
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glBindFragDataLocation(shaderProgram, 0, "outColor");
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram);

    // Specify the layout of the vertex data
    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), 0);
    //GLint uniColor = glGetUniformLocation(shaderProgram, "triangleColor");
    //glUniform3f(uniColor, 1.0f, 0.0f, 0.0f);

    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
                          7 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));

    GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
    glEnableVertexAttribArray(texAttrib);
    glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE,
                          7 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));

    glUniform1i(glGetUniformLocation(shaderProgram, "texKitten"), 0);
    glUniform1i(glGetUniformLocation(shaderProgram, "texPuppy"), 1);

    auto t_start = std::chrono::high_resolution_clock::now();
    GLint uniTime = glGetUniformLocation(shaderProgram, "time");

    GLint uniModel = glGetUniformLocation(shaderProgram, "model");

    glm::mat4 view = glm::lookAt(
            glm::vec3(4.0f, 4.0f, 3.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 0.0f, 1.0f)
    );
    GLint uniView = glGetUniformLocation(shaderProgram, "view");
    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

    glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 1.0f, 10.0f);
    GLint uniProj = glGetUniformLocation(shaderProgram, "proj");
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

    bool running = true;
    while (running)
    {
        sf::Event windowEvent;
        while (window.pollEvent(windowEvent))
        {
            switch (windowEvent.type)
            {
                case sf::Event::Closed:
                    running = false;
                    break;
                case sf::Event::KeyPressed:
                    if (windowEvent.key.code == sf::Keyboard::Escape)
                        running = false;
                    break;
            }
        }

        // Clear the screen to black
        //glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        //glClear(GL_COLOR_BUFFER_BIT);

        auto t_now = std::chrono::high_resolution_clock::now();
        float time = std::chrono::duration_cast<std::chrono::duration<float>>(t_now - t_start).count();
        glUniform1f(uniTime, time);
        //glUniform3f(uniColor, (sin(time * 4.0f) + 1.0f) / 2.0f, 0.0f, 0.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(
                model,
                time * glm::radians(180.0f),
                glm::vec3(0.0f, 0.0f, 1.0f)
        );

        GLfloat s = sin(time * 5.0f) * 0.25f + 0.75f;
        model = glm::scale(model, glm::vec3(s, s, s));

        glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers
        window.display();
    }
    glDeleteProgram(shaderProgram);
    glDeleteShader(fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteBuffers(1, &ebo);
    glDeleteBuffers(1, &vbo);
    glDeleteTextures(1, textures);
    glDeleteVertexArrays(1, &vao);

    window.close();
    return 0;
}