#include <box_renderer.h>

// manages player movement

class Movement {
private:
	// stores pointer to the box to directly modify it
	BoxRenderer::Box* box_;
	// player should move in a grid
	bool moving_ = false;
	BoxRenderer::Vec2 move_ = { 0, 0 };
	BoxRenderer::Vec2 destination_ = { 0, 0 };
	float speed_;
	float w_unit_;
	float h_unit_;

public:
	Movement(BoxRenderer::Box& box, float& speed, float& w_unit, float& h_unit) {
		box_ = &box;
		speed_ = speed;
		w_unit_ = w_unit;
		h_unit_ = h_unit;
	}

	void set_move(const BoxRenderer::Vec2& move) {
		// don't want to overwrite in the middle of movement
		if (moving_) return;
		moving_ = true;
		move_ = move;
		destination_ = { (*box_).position().x + move.x*w_unit_, (*box_).position().y + move.y*h_unit_ };
	}

	const BoxRenderer::Vec2& position() const {
		return (*box_).position();
	}

	void update(const float dt) {
		// if not moving nothing to do
		if (!moving_) return;

		// values of move will be either 1 or -1 for both coordinates
		// we move on one coordinate at a time
		if (move_.x != 0) {
			(*box_).position().x += move_.x * speed_ * dt;
			// left
			if (move_.x < 0 && (*box_).position().x < destination_.x) {
				(*box_).position().x = destination_.x;
				moving_ = false;
				return;
			}
			// right
			if (move_.x > 0 && (*box_).position().x > destination_.x) {
				(*box_).position().x = destination_.x;
				moving_ = false;
				return;
			}

		}

		if (move_.y != 0) {
			(*box_).position().y += move_.y * speed_ * dt;
			// down
			if (move_.y < 0 && (*box_).position().y < destination_.y) {
				(*box_).position().y = destination_.y;
				moving_ = false;
				return;
			}
			// up
			if (move_.y > 0 && (*box_).position().y > destination_.y) {
				(*box_).position().y = destination_.y;
				moving_ = false;
				return;
			}
		}
	}
};