#include <iostream>
#include <box_renderer.h>
#include "source/movement.cpp"

int main()
{   
    std::cout << "Hello World!" << std::endl;
    // settings
    constexpr unsigned int SCR_WIDTH = 600;
    constexpr unsigned int SCR_HEIGHT = 800;

    // canvas coordinates x,y go from -1 to 1
    BoxRenderer::Canvas canvas = BoxRenderer::Canvas(SCR_WIDTH, SCR_HEIGHT, "Frogger Lite");

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
    BoxRenderer::BoxId frog_box = canvas.addBox({ {0.f, 0.f}, BoxRenderer::Color::Green(), { 2.0f/g_columns, 2.0f/g_rows } });
    Movement player(frog_box);

    bool moveRight = false;
    bool movingRight = true;
    float speed = 0.0001;

    auto update = [&](float dt)
    {
        BoxRenderer::Box& box = canvas.getBox(player.id());

        if (movingRight)
            box.position().x += speed * dt;
        else
            box.position().x -= speed * dt;

        if (box.position().x > 0.5)
            movingRight = false;
        else if (box.position().x < -0.5)
            movingRight = true;
    };

    // generate lambda functions for player movement
    auto up = [&player, &h_unit]() { player.set_move({ h_unit, 0.f }); };
    auto down = [&player, &h_unit]() { player.set_move({ h_unit, 0.f }); };
    auto left = [&player, &w_unit]() { player.set_move({ 0.f, -w_unit }); };
    auto right = [&player, &w_unit]() { player.set_move({ 0.f, w_unit }); };


    Alice::Controller controller;
    controller.onKeyPress(Alice::Key::W, up);
    controller.onKeyPress(Alice::Key::A, down);
    controller.onKeyPress(Alice::Key::S, left);
    controller.onKeyPress(Alice::Key::D, right);
    controller.onKeyPress(Alice::Key::SPACE, [&]() { speed = 0.0f; });
    controller.onKeyPress(Alice::Key::ESCAPE, [&]() { canvas.close(); });

    canvas.drawScene(controller, update); //runScene

    return 0;
}