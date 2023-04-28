#include <box_renderer.h>
#include <map>

// wrapper class for Box that gives it movement functionality for player input
class Player {
private:
	// stores pointer to the box to directly modify it
	BoxRenderer::Canvas* canvas_;
	// player should move in a grid
	bool moving_ = false;
	BoxRenderer::Vec2 move_ = { 0, 0 };
	BoxRenderer::Vec2 destination_ = { 0, 0 };
	float speed_;
	float w_unit_;
	float h_unit_;
	// here we store all the boxes that make up the frog
	// both coordinates are offset
	std::map<std::pair<float, float>, BoxRenderer::BoxId> parts_;

public:
	Player(BoxRenderer::Canvas* canvas, float& speed, const float& w_unit, const float& h_unit) {
		canvas_ = canvas;
		speed_ = speed;
		w_unit_ = w_unit;
		h_unit_ = h_unit;
		// building the frog:
		// frog has a fixed size, so we don't need to create a hitbox (unlike cars)
		// arms
		parts_[{0.f, 0.f}] = canvas_->addBox({ {0.f, 0.f}, BoxRenderer::Color::Black(), {w_unit_ * 0.001f, h_unit_ * 0.001f} });
		// arms/legs (symmetric)
		// parts_[{0.f, 0.f}] = canvas_->addBox({ {0.f, 0.f}, BoxRenderer::Color::Color(0, 0.5, 0), {w_unit_ * 0.8f, h_unit_ * 0.8f} });
		parts_[{0.f, h_unit * 0.19f}] = canvas_->addBox({ {0.f, 0.f}, BoxRenderer::Color::Color(0, 0.55, 0), { w_unit_ * 0.65f, h_unit_ * 0.4f} });
		parts_[{0.f, -h_unit * 0.19f}] = canvas_->addBox({ {0.f, 0.f}, BoxRenderer::Color::Color(0, 0.55, 0), { w_unit_ * 0.8f, h_unit_ * 0.4f } });
		// body (2 parts)
		float body_offset_y = h_unit_ * 0.2f;
		parts_[{0.f, body_offset_y}] = canvas_->addBox({ {0.f, body_offset_y}, BoxRenderer::Color::Green(), { w_unit_ * 0.4f, h_unit_*0.4f } });
		parts_[{0.f, -body_offset_y}] = canvas_->addBox({ {0.f, -body_offset_y}, BoxRenderer::Color::Green(), { w_unit_ * 0.6f, h_unit_*0.4f } });
		// eyes (symmetric)
		parts_[{w_unit_ * 0.2f, body_offset_y}] = canvas_->addBox({ {w_unit_ * 0.2f, body_offset_y}, BoxRenderer::Color::White(), { w_unit_ * 0.1f, h_unit_ * 0.2f} });
		parts_[{-w_unit_ * 0.2f, body_offset_y}] = canvas_->addBox({ {-w_unit_ * 0.2f, body_offset_y}, BoxRenderer::Color::White(), { w_unit_ * 0.1f, h_unit_ * 0.2f} });
	}

	const bool& moving() const {
		return moving_;
	}

	void initial_pos(const BoxRenderer::Vec2& pos) {
		for (auto& [offset, id] : parts_) {
			BoxRenderer::Box& part = canvas_->getBox(id);
			part.position().x = pos.x + offset.first;
			part.position().y = pos.y + offset.second;
		}
	}

	void set_move(const BoxRenderer::Vec2& move) {
		// don't want to overwrite in the middle of movement
		if (moving_) return;
		destination_ = { canvas_->getBox(parts_[{0.f, 0.f}]).position().x + move.x * w_unit_, canvas_->getBox(parts_[{0.f, 0.f}]).position().y + move.y * h_unit_ };
		// player can't go out of bounds (off-screen)
		// it's not necessary to check y>1 as it triggers the win condition
		if (destination_.x < -1 || destination_.x > 1 || destination_.y < -1) {
			moving_ = false;
			return;
		}
		moving_ = true;
		move_ = move;
	}

	const BoxRenderer::Vec2& position() {
		return canvas_->getBox(parts_[{0.0f, 0.0f}]).position();
	}

	void update(const float dt) {
		// if not moving nothing to do
		if (!moving_) return;

		// after moving the parts
		// check if we moved past the destination
		// if so, snap back to it
		// for this we use the central piece
		BoxRenderer::Box& center = canvas_->getBox(parts_[{0.f, 0.f}]);

		// values of move will be either 1 or -1 for both coordinates
		// we move on one coordinate at a time, so we nest the for inside
		// to avoid doing the move check for every part
		if (move_.x != 0) {
			for (auto& [offset, id] : parts_) {
				BoxRenderer::Box& part = canvas_->getBox(id);
				part.position().x += move_.x * speed_ * dt;
			}

			// destination check
			if (move_.x < 0 && center.position().x < destination_.x
				|| move_.x > 0 && center.position().x > destination_.x) {
				moving_ = false;
				for (auto& [offset, id] : parts_) {
					canvas_->getBox(id).position().x = destination_.x + offset.first;
				}
				return;
			}
		}

		if (move_.y != 0) {
			for (auto& [offset, id] : parts_) {
				BoxRenderer::Box& part = canvas_->getBox(id);
				part.position().y += move_.y * speed_ * dt;
			}
			// destination check
			if (move_.y < 0 && center.position().y < destination_.y
				|| move_.y > 0 && center.position().y > destination_.y) {
				moving_ = false;
				for (auto& [offset, id] : parts_) {
					canvas_->getBox(id).position().y = destination_.y + offset.second;
				}
				return;
			}
		}
	}
};