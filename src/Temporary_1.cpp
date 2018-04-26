#include <iostream>
#include <Temporary_4.h>
using namespace std;

int main()
{
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;

    sf::Window window(sf::VideoMode(WIDTH, HEIGHT, 24), "Transform Feedback", sf::Style::Titlebar | sf::Style::Close, settings);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    Model model("/home/leon/Instrument_Pose_Estimation/Test_C", "Iron_Man_mark_6.dae");// Iron_Man_mark_6.dae  model_simplified.sdf

    //Model room("/home/leon/Instrument_Pose_Estimation/Test_C","room.dae", false);

    // run the main loop
    bool running = true;
    VectorXd pose_estimator(6);
    pose_estimator << 0,0,-1,0,0,0;

    char k;

    while (running && k!=32)
    {
        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    running = false;
                    break;
                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                        running = false;
                    break;
                case sf::Event::Resized:
                    glViewport(0, 0, event.size.width, event.size.height);
                    break;
            }
        }

        VectorXd pose(6),grad(6);
        pose << 0,0,-1,degreesToRadians(0),degreesToRadians(0),degreesToRadians(0);
        Mat img_camera;

        cout << "press ENTER to run tracking, press SPACE to toggle first person view (use WASD-keys to move around)" << endl;
        while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
            model.updateViewMatrix(window);
            model.render(img_camera, false, "color_simple");
            window.display();
        }


    }
}