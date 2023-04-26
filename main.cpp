#include <iostream>
#include <box_renderer.h>
#include <vector>
#include "source/movement.cpp"
#include "source/car.cpp"

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

    // we want a 10x5 grid, we need to calculate positions
    int g_columns = 7;
    int g_rows = 10;

    float w_unit = 2.f / g_columns;
    float h_unit = 2.f / g_rows;

    // to win we need to get to the top, no matter the x position
    // so we only care about y
    float y_win = 1 - h_unit / 2;

    float car_speed = 0.001;
    float player_speed = 0.002;

    
    
    // adding cars (sizes are 1 and 3 grid cells)
    BoxRenderer::BoxId car_test = canvas.addBox({ {0.f, 0.f}, BoxRenderer::Color::Red(), { 2.0f / g_columns, 2.0f / g_rows } });


    // frog size is 1 grid cell
    // we want frog to start in the middle of the bottom row
    // added last so it's drawn on top of everything else
    BoxRenderer::BoxId frog_id = canvas.addBox({ {0.f, -1 + h_unit / 2.f}, BoxRenderer::Color::Green(), { 2.0f / g_columns, 2.0f / g_rows } });

    // getBox must be called after adding all boxes
    Movement player(canvas.getBox(frog_id), player_speed, w_unit, h_unit);
    Car car1(canvas.getBox(car_test), car_speed, w_unit, h_unit, -1);
    std::vector<Car> car_vec;
    car_vec.push_back(car1);
    auto update = [&player, &canvas, &y_win, &car_vec](float dt)
    {
        for (Car car : car_vec) {
            car.update(dt);
        }
        player.update(dt);

        // win condition
        if (player.position().y == y_win) {
            std::cout << "victory!" << std::endl;
        }
    };
  

    // Player Input
    // generate lambda functions
    auto up = [&player, &h_unit]() { player.set_move({ 0.f, 1 }); };
    auto down = [&player, &h_unit]() { player.set_move({ 0.f, -1 }); };
    auto left = [&player, &w_unit]() { player.set_move({ -1, 0.f }); };
    auto right = [&player, &w_unit]() { player.set_move({ 1, 0.f }); };


    Alice::Controller controller;
    controller.onKeyPress(Alice::Key::W, up);
    controller.onKeyPress(Alice::Key::A, left);
    controller.onKeyPress(Alice::Key::S, down);
    controller.onKeyPress(Alice::Key::D, right);
    controller.onKeyPress(Alice::Key::ESCAPE, [&canvas]() { canvas.close(); });

    canvas.drawScene(controller, update); //runScene

    return 0;
}