#include <iostream>
#include <box_renderer.h>
#include "source/movement.cpp"
#include "source/car.cpp"
#include "source/sound_manager.cpp"

int main()
{   
    // audio manager using miniaudio library
    SoundManager soundManager;

    // start to play the scheduled notes
    soundManager.playNext();

    std::cout << "Welcome to Frogger Lite!" << std::endl;
    // settings
    constexpr unsigned int SCR_WIDTH = 600;
    constexpr unsigned int SCR_HEIGHT = 800;

    // canvas coordinates x,y go from -1 to 1
    BoxRenderer::Canvas canvas = BoxRenderer::Canvas(SCR_WIDTH, SCR_HEIGHT, "Frogger Lite");

    // black background
    canvas.setBackgroundColor(BoxRenderer::Color::Black());

    // we want a 10x5 grid, we need to calculate positions
    int g_columns = 10;
    int g_rows = 12;

    float w_unit = 2.f / g_columns;
    float h_unit = 2.f / g_rows;

    // to win we need to get to the top, no matter the x position
    // so we only care about y
    float y_win = 1 - h_unit / 2;

    float car_speed = 0.001;
    float player_speed = 0.002;
    
    // adding cars (sizes are 1 and 3 grid cells)
    // first and last rows can't have cars
    // position needs to be at the middle of a grid cell
    float row1 = -1 + h_unit / 2;
    float col1 = -1 + w_unit / 2;
   
    Car car1(&canvas, car_speed, w_unit, h_unit, -1);
    car1.move({ -0.5, 0.5 });

    Car car2(&canvas, car_speed, w_unit, h_unit, -1, 3);


    // frog size is 1 grid cell
    // we want frog to start in the middle of the bottom row
    // added last so it's drawn on top of everything else
    // 
    BoxRenderer::BoxId frog_id = canvas.addBox({ {col1 + w_unit * g_columns / 2, row1}, BoxRenderer::Color::Green(), { w_unit, h_unit} });

    // getBox must be called after adding all boxes
    Movement player(canvas.getBox(frog_id), player_speed, w_unit, h_unit);

    std::vector<Car> car_vec;
    car_vec.push_back(car1);
    car_vec.push_back(car2);

    bool end = false;
    auto update = [&player, &canvas, &y_win, &car_vec, &h_unit, &w_unit, &soundManager, &end](float dt)
    {
        // audio
        soundManager.tick(dt);

        if (!end) {
            player.update(dt);

            // collision check for new cars
            for (Car car : car_vec) { //placeholder for eventual new car list
                car.update(dt);
                if (car.check_y_collision(player.position().y)) {
                    // cars can be w_unit or 3*w_unit wide, so we check using the corners
                    if (car.check_x_collision(player.position().x)) {
                        // queue game over jingle
                        soundManager.scheduleNote(Note{ 200, 0.1, 195.998 });
                        soundManager.scheduleNote(Note{ 500, 0.1, 100.563 });
                        //std::cout << "collision! car: " << car.position() << " player: " << player.position() << std::endl;
                        end = true;
                    }
                }
            }

            // win condition (accounting for float inaccurracies)
            if (player.position().y + 0.0001 >= y_win) {
                // queue victory jingle
                soundManager.scheduleNote(Note{ 250, 0.1, 391.995 });
                soundManager.scheduleNote(Note{ 250, 0.1, 440.000 });
                soundManager.scheduleNote(Note{ 600, 0.1, 493.883 });
                // std::cout << "Victory!" << std::endl;
                end = true;
            }
        }
    };
  

    // player movement note
    Note n_move{ 120, 0.1, 261.626 };

    // make a common note play function for all player inputs
    auto play_move = [&soundManager, &n_move, &end]() {if (end) return; soundManager.scheduleNote(n_move); };

    // Player Input
    // generate lambda functions
    auto up = [&player, &play_move]() { play_move(); player.set_move({ 0.f, 1.f }); };
    auto down = [&player, &play_move]() { play_move(); player.set_move({ 0.f, -1.f }); };
    auto left = [&player, &play_move]() { play_move(); player.set_move({ -1.f, 0.f }); };
    auto right = [&player, &play_move]() { play_move(); player.set_move({ 1.f, 0.f }); };


    Alice::Controller controller;
    controller.onKeyPress(Alice::Key::W, up);
    controller.onKeyPress(Alice::Key::A, left);
    controller.onKeyPress(Alice::Key::S, down);
    controller.onKeyPress(Alice::Key::D, right);
    controller.onKeyPress(Alice::Key::ESCAPE, [&canvas]() { canvas.close(); });

    canvas.drawScene(controller, update); //runScene

    return 0;
}