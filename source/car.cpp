#include <box_renderer.h>
#include <map>

// class generates a car in a given Canvas
class Car {
private:
	// stores pointer to the canvas to generate car in it
	BoxRenderer::Canvas* canvas_ = NULL;
	// car moves in a fixed direction, doesn't care about columns
	// only the row it's in, so dir is either 1 (to the right) or -1 (to the left)
	int d_ = 1;
	int size_ = 1;
	float speed_ = 0.f;
	float w_unit_ = 0.f;
	float h_unit_ = 0.f;
	float current_y_ = 0.f;
	// here we store all the boxes that make up the car
	std::map<float, BoxRenderer::BoxId> parts_;

public:
	// regular car
	Car(BoxRenderer::Canvas* canvas, float speed, float& w_unit, float& h_unit, int dir) {
		canvas_ = canvas;
		w_unit_ = w_unit;
		h_unit_ = h_unit;
		d_ = dir;
		speed_ = speed;
		// building the car:
		// hitbox
		parts_[0.f] = canvas_->addBox({ {0.f, 0.f}, BoxRenderer::Color::Black(), { w_unit_, h_unit_ } });
		// wheels (independent of dir)
		parts_[-w_unit_ * 0.3f] = canvas_->addBox({ {-w_unit_ * 0.3f, 0.f}, BoxRenderer::Color::Gray(), { w_unit_ * 0.3f, h_unit_ } });
		parts_[w_unit_ * 0.3f] = canvas_->addBox({ {w_unit_ * 0.3f, 0.f}, BoxRenderer::Color::Gray(), { w_unit_ * 0.3f, h_unit_ } });
		// car body (2 parts)
		parts_[-d_ * w_unit_ / 4] = canvas_->addBox({ {-d_ * w_unit_ / 4, 0.f}, BoxRenderer::Color::Red(), { w_unit_ / 2, h_unit_ * 0.9f } });
		parts_[d_ * w_unit_ / 4] = canvas_->addBox({ {d_*w_unit_ / 4, 0.f}, BoxRenderer::Color::Red(), { w_unit_ / 2, h_unit_ * 0.8f } });
		// car window
		parts_[d_ * w_unit_ * 0.2f] = canvas_->addBox({ {d_*w_unit_*0.2f, 0.f}, BoxRenderer::Color::Color(0,1,1), { w_unit_*0.3f, h_unit_ * 0.5f} });
	}
	// if we specify size, we create a different car
	Car(BoxRenderer::Canvas* canvas, float speed, float& w_unit, float& h_unit, int dir, int size) {
		canvas_ = canvas;
		w_unit_ = w_unit;
		h_unit_ = h_unit;
		d_ = dir;
		speed_ = speed;
		size_ = size;
		// building the car:
		// hitbox
		parts_[0.f] = canvas_->addBox({ {0.f, 0.f}, BoxRenderer::Color::Black(), { size_*w_unit_, h_unit_ } });
		// wheels (independent of dir)
		parts_[size_ * -w_unit_ * 0.3f] = canvas_->addBox({ {size_ * -w_unit_ * 0.3f, 0.f}, BoxRenderer::Color::White(), { w_unit_ * 0.25f, h_unit_ } });
		parts_[size_ * w_unit_ * 0.3f] = canvas_->addBox({ {size_ * w_unit_ * 0.3f, 0.f}, BoxRenderer::Color::White(), { w_unit_ * 0.25f, h_unit_ } });
		// car body (2 parts)
		parts_[-d_ * size_ * w_unit_ / 4] = canvas_->addBox({ {-d_ * size_ * w_unit_ / 4, 0.f}, BoxRenderer::Color::Gray(), { 2* size_ * w_unit_ / 3, h_unit_ * 0.9f } });
		parts_[d_ * size_ * w_unit_ / 4] = canvas_->addBox({ {d_ * size_ * w_unit_ / 4, 0.f}, BoxRenderer::Color::Yellow(), { size_ * w_unit_ /3, h_unit_ * 0.8f } });
		// car window
		parts_[d_ * size_ * w_unit_ * 0.2f] = canvas_->addBox({ {d_ * size_ * w_unit_ * 0.2f, 0.f}, BoxRenderer::Color::Blue(), { size_ * w_unit_ * 0.15f, h_unit_ * 0.5f} });
	}

	// set up the position of the car before update loop
	void move(const BoxRenderer::Vec2 pos) {
		current_y_ = pos.y;
		for (auto &[offset, id] : parts_) {
			BoxRenderer::Box& part = canvas_->getBox(id);
			part.position().x = pos.x + offset;
			part.position().y = pos.y;
		}
	}

	// check collision with player
	const bool& check_y_collision(const float& player_y) const {
		return (current_y_ - h_unit_ / 2 < player_y && player_y < current_y_ + h_unit_ / 2);
	}

	// for x we take into account size
	const bool& check_x_collision(const float& player_x) {
		return (canvas_->getBox(parts_[0.f]).position().x - size_* w_unit_ / 2 < player_x &&
			player_x < canvas_->getBox(parts_[0.f]).position().x + size_ * w_unit_ / 2);
	}

	void update(const float dt) {
		// values of move will be either 1 or -1 for both coordinates
		// cars only move on x and are always moving

		for (auto &[offset, id] : parts_) {
			BoxRenderer::Box& part = canvas_->getBox(id);
			part.position().x += d_ * speed_ * dt;
		}
		// we determine if the car is offscreen by the position of the hitbox
		// left
		if (canvas_->getBox(parts_[0.f]).position().x < -3) {
			// if so, we move all parts back
			for (auto &[offset, id] : parts_) {
				BoxRenderer::Box& part = canvas_->getBox(id);
				part.position().x = 2.5 + offset;
				//std::cout << "went offscreen to the left" << std::endl;
			}
		}
		// right
		if (canvas_->getBox(parts_[0.f]).position().x > 3) {
			for (auto &[offset, id] : parts_) {
				BoxRenderer::Box& part = canvas_->getBox(id);
				part.position().x = -2.5 + offset;
				//std::cout << "went offscreen to the right" << std::endl;
			}
		}
	}
};