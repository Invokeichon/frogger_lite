#include <iostream>
#include <box_renderer.h>

int main()
{   
    std::cout << "Hello World!" << std::endl;
    // settings
    constexpr unsigned int SCR_WIDTH = 600;
    constexpr unsigned int SCR_HEIGHT = 800;

    // canvas coordinates x,y go from -1 to 1
    BoxRenderer::Canvas canvas(SCR_WIDTH, SCR_HEIGHT, "Frogger Lite");

    // black background
    canvas.setBackgroundColor(BoxRenderer::Color::Black());

    // we want a 10x5 grid, we need to calculate position
    int g_columns = 5;
    int g_rows = 10;

    // 
    float w_unit = 2.f / 5;
    float h_unit = 2.f / 10;
    std::cout << w_unit << std::endl;
    
    // fros should position itself in the middle of a grid unit
    // we want frog to start in the middle of the bottom row
    float frog_start = (-1 + h_unit/2.f);
    std::cout << frog_start << std::endl;

    // frog size should be 1 grid unit
    BoxRenderer::BoxId frog = canvas.addBox({ {0.0f, frog_start}, BoxRenderer::Color::Green(), { 2.0f/g_columns, 2.0f/g_rows } });

    bool movingRight = true;
    float speed = 0.0001;

    auto update = [&](float dt)
    {
        BoxRenderer::Box& box = canvas.getBox(frog);

        if (movingRight)
            box.position().x += speed * dt;
        else
            box.position().x -= speed * dt;

        if (box.position().x > 0.5)
            movingRight = false;
        else if (box.position().x < -0.5)
            movingRight = true;
    };

    // user input
    Alice::Controller controller;
    controller.onKeyPress(Alice::Key::W, [&]() { speed *= 2.0f; });
    controller.onKeyPress(Alice::Key::A, [&]() { speed += 3.0f; });
    controller.onKeyPress(Alice::Key::S, [&]() { speed /= 2.0f; });
    controller.onKeyPress(Alice::Key::D, [&]() { speed -= 3.0f; });
    controller.onKeyPress(Alice::Key::SPACE, [&]() { speed = 0.0f; });
    controller.onKeyPress(Alice::Key::ESCAPE, [&]() { canvas.close(); });

    canvas.drawScene(controller, update); //runScene

    return 0;
}