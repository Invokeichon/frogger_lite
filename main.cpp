#include <iostream>
#include <box_renderer.h>
#include "source/player.cpp"
#include "source/car.cpp"
#include "source/sound_manager.cpp"

int main()
{   
    // audio manager using miniaudio library
    SoundManager soundManager;

    // start to play the scheduled notes
    soundManager.playNext();

    // schedule a beginning jingle
    soundManager.scheduleNote(Note{ 150, 0.1, 440.000 });
    soundManager.scheduleNote(Note{ 150, 0.1, 493.883 });
    soundManager.scheduleNote(Note{ 300, 0.1, 523.251 });

    std::cout << "Welcome to Frogger Lite!" << std::endl;
    // window settings
    constexpr unsigned int SCR_WIDTH = 600;
    constexpr unsigned int SCR_HEIGHT = 800;
    // grid settings
    constexpr unsigned int GRID_COLS = 10;
    constexpr unsigned int GRID_ROWS = 13;

    // canvas coordinates x,y go from -1 to 1
    BoxRenderer::Canvas canvas = BoxRenderer::Canvas(SCR_WIDTH, SCR_HEIGHT, "Frogger Lite");

    // black background
    canvas.setBackgroundColor(BoxRenderer::Color::Black());

    constexpr float CELL_WIDTH = 2.f / GRID_COLS;
    constexpr float CELL_HEIGHT = 2.f / GRID_ROWS;

    // to win we need to get to the top, no matter the x position
    // so we only care about y
    float y_win = 1 - CELL_HEIGHT / 2;

    float car_speed = 0.001f;
    float player_speed = 0.0015f;
    
    // adding cars (sizes are 1 and 3 grid cells)
    // first and last rows can't have cars
    // position needs to be at the middle of a grid cell
    float row1 = -1 + CELL_HEIGHT / 2;
    float col1 = -1 + CELL_WIDTH / 2;
    
    // add dark green blocks covering the rows to indicate safe zones
    canvas.addBox({ {0.f, row1}, BoxRenderer::Color::Color(0, 0.2, 0), { 2, CELL_HEIGHT } });
    canvas.addBox({ {0.f, -row1}, BoxRenderer::Color::Color(0, 0.2, 0), { 2, CELL_HEIGHT } });

    // from rows 2 to 12 we have cars
    // row 2 -- a speedy car
    Car row2_1(&canvas, car_speed*2, CELL_WIDTH, CELL_HEIGHT, -1);
    row2_1.initial_pos({ -0.5f, row1 + CELL_HEIGHT });

    // row 3 -- a truck
    Car row3_1(&canvas, car_speed*1.1, CELL_WIDTH, CELL_HEIGHT, 1, 3);
    row3_1.initial_pos({0.f, row1 + 2*CELL_HEIGHT});

    // row 4 -- 2 trucks
    Car row4_1(&canvas, car_speed, CELL_WIDTH, CELL_HEIGHT, -1, 3);
    Car row4_2(&canvas, car_speed, CELL_WIDTH, CELL_HEIGHT, -1, 3);
    row4_1.initial_pos({ 0.2f, row1 + 3 * CELL_HEIGHT });
    row4_2.initial_pos({ -0.5f, row1 + 3 * CELL_HEIGHT });

    // row 5 -- two slightly fast cars spaced
    Car row5_1(&canvas, car_speed * 1.3, CELL_WIDTH, CELL_HEIGHT, -1);
    Car row5_2(&canvas, car_speed * 1.3, CELL_WIDTH, CELL_HEIGHT, -1);
    row5_1.initial_pos({ -0.3f, row1 + 4 * CELL_HEIGHT });
    row5_2.initial_pos({ 0.3f, row1 + 4 * CELL_HEIGHT });

    // row 6 -- a speedy truck
    Car row6_1(&canvas, car_speed * 2, CELL_WIDTH, CELL_HEIGHT, 1, 3);
    row6_1.initial_pos({ -0.6f, row1 + 5 * CELL_HEIGHT });

    // no cars in row 7 --- a little rest for the player :)
    // add a dark green block to indicate safe zone
    canvas.addBox({ {0.f, row1 + 6 * CELL_HEIGHT}, BoxRenderer::Color::Color(0, 0.2, 0), { 2, CELL_HEIGHT } });

    // row 8 -- 3 cars
    Car row8_1(&canvas, car_speed * 1.3, CELL_WIDTH, CELL_HEIGHT, -1);
    Car row8_2(&canvas, car_speed * 1.3, CELL_WIDTH, CELL_HEIGHT, -1);
    Car row8_3(&canvas, car_speed * 1.3, CELL_WIDTH, CELL_HEIGHT, -1);
    row8_1.initial_pos({ -1.f, row1 + 7 * CELL_HEIGHT });
    row8_2.initial_pos({ 0.f, row1 + 7 * CELL_HEIGHT });
    row8_3.initial_pos({ 1.f, row1 + 7 * CELL_HEIGHT });

    // row 9 - 1 SLOW truck
    Car row9_1(&canvas, car_speed*0.5f, CELL_WIDTH, CELL_HEIGHT, -1, 3);
    row9_1.initial_pos({ -0.5f, row1 + 8 * CELL_HEIGHT });

    // row 10 - 2 slightly fast trucks spaced
    Car row10_1(&canvas, car_speed * 0.5f, CELL_WIDTH, CELL_HEIGHT, 1, 3);
    Car row10_2(&canvas, car_speed * 0.5f, CELL_WIDTH, CELL_HEIGHT, 1, 3);
    row10_1.initial_pos({ 1.f, row1 + 9 * CELL_HEIGHT });
    row10_2.initial_pos({ -1.f, row1 + 9 * CELL_HEIGHT });

    // row 11 -- FAST car
    Car row11_1(&canvas, car_speed * 2.3, CELL_WIDTH, CELL_HEIGHT, -1);
    row11_1.initial_pos({ 1.5f, row1 + 10 * CELL_HEIGHT });

    // row 12 -- four spaced cars
    Car row12_1(&canvas, car_speed * 1.1, CELL_WIDTH, CELL_HEIGHT, 1);
    Car row12_2(&canvas, car_speed * 1.1, CELL_WIDTH, CELL_HEIGHT, 1);
    Car row12_3(&canvas, car_speed * 1.1, CELL_WIDTH, CELL_HEIGHT, 1);
    Car row12_4(&canvas, car_speed * 1.1, CELL_WIDTH, CELL_HEIGHT, 1);
    row12_1.initial_pos({ 1.5f, row1 + 11 * CELL_HEIGHT });
    row12_2.initial_pos({ 0.75f, row1 + 11 * CELL_HEIGHT });
    row12_3.initial_pos({ -0.75f, row1 + 11 * CELL_HEIGHT });
    row12_4.initial_pos({ -1.5f, row1 + 11 * CELL_HEIGHT });

    // frog size is 1 grid cell
    // we want frog to start in the middle of the bottom row
    // added last so it's drawn on top of everything else

    // getBox must be called after adding all boxes
    Player player(&canvas, player_speed, CELL_WIDTH, CELL_HEIGHT);
    player.initial_pos({ col1 + CELL_WIDTH * GRID_COLS / 2, row1 });

    // cars are put in a vector for convenience
    std::vector<Car> car_vec ={ row2_1, row3_1, row4_1, row4_2, row5_1, row5_2, row6_1, row8_1, row8_2, row8_3, 
        row9_1, row10_1, row10_2, row11_1, row12_1, row12_2, row12_3, row12_4};

    // variable for end conditions
    bool end = false;
    auto update = [&player, &canvas, &y_win, &car_vec, &soundManager, &end](float dt)
    {
        // audio
        soundManager.tick(dt);

        if (!end) {
            player.update(dt);

            // collision check for cars
            for (Car car : car_vec) {
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
    auto up = [&player, &play_move]() { if (player.moving()) return; play_move(); player.set_move({ 0.f, 1.f }); };
    auto down = [&player, &play_move]() { if (player.moving()) return; play_move(); player.set_move({ 0.f, -1.f }); };
    auto left = [&player, &play_move]() { if (player.moving()) return; play_move(); player.set_move({ -1.f, 0.f }); };
    auto right = [&player, &play_move]() { if (player.moving()) return; play_move(); player.set_move({ 1.f, 0.f }); };


    Alice::Controller controller;
    controller.onKeyPress(Alice::Key::W, up);
    controller.onKeyPress(Alice::Key::A, left);
    controller.onKeyPress(Alice::Key::S, down);
    controller.onKeyPress(Alice::Key::D, right);
    controller.onKeyPress(Alice::Key::ESCAPE, [&canvas]() { canvas.close(); });

    canvas.drawScene(controller, update); //runScene

    return 0;
}