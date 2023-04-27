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
    BoxRenderer::BoxId car_id1 = canvas.addBox({ {0.f, 5*h_unit/2}, BoxRenderer::Color::Red(), { w_unit, h_unit } });
    BoxRenderer::BoxId car_id2 = canvas.addBox({ {0.f, 3*h_unit/2}, BoxRenderer::Color::Gray(), { w_unit * 3, h_unit} });


    // frog size is 1 grid cell
    // we want frog to start in the middle of the bottom row
    // added last so it's drawn on top of everything else
    BoxRenderer::BoxId frog_id = canvas.addBox({ {0.f, -1 + h_unit / 2.f}, BoxRenderer::Color::Green(), { w_unit, h_unit} });

    // getBox must be called after adding all boxes
    Movement player(canvas.getBox(frog_id), player_speed, w_unit, h_unit);
    Car car1(canvas.getBox(car_id1), car_speed, w_unit, h_unit, -1);
    Car car2(canvas.getBox(car_id2), car_speed, w_unit, h_unit, 1);
    std::vector<Car> car_vec;
    car_vec.push_back(car1);
    car_vec.push_back(car2);
    auto update = [&player, &canvas, &y_win, &car_vec, &h_unit, &w_unit](float dt)
    {
        player.update(dt);

        // collision check
        for (Car car : car_vec) {
            car.update(dt);
            // idea: check upper-right and lower-left corners of car
            // then do comparison to player
            // all cars are h_unit tall
            if (car.position().y-h_unit/2 < player.position().y && player.position().y < car.position().y+h_unit/2) {
                // cars can be w_unit or 3*w_unit wide, so we check using the corners
                if (car.bottomLeft().x < player.position().x && player.position().x < car.upperRight().x ){
                    std::cout << "collision" << car.position() << player.position() << std::endl;
                }
            }
        }

        // win condition (accounting for float inaccurracies)
        if (player.position().y + 0.0001 >= y_win) {
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