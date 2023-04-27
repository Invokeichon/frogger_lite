#include <box_renderer.h>

// class meant to wrap BoxRenderer boxes to add game functionality
class Car {
private:
	// stores pointer to the box to directly modify it
	BoxRenderer::Box* box_;
	// car moves in a fixed direction, doesn't respect the grid except for y
	// we only give dir, either 1 or -1
	int dir_ = 1;
	float speed_;
	float w_unit_;
	float h_unit_;

public:
	Car(BoxRenderer::Box& box, float speed, float& w_unit, float& h_unit, int dir) {
		box_ = &box;
		dir_ = dir;
		speed_ = speed;
		w_unit_ = w_unit;
		h_unit_ = h_unit;
	}

	const BoxRenderer::Vec2& position() const {
		return box_->position();
	}

	BoxRenderer::Vec2 upperRight() const {
		// calc was relative, we add position to get actual corner
		return BoxRenderer::Vec2{ box_->upperRight().x + box_->position().x, box_->upperRight().y + box_->position().y };
	}

	BoxRenderer::Vec2 bottomLeft() const {
		// calc was relative, we add position to get actual corner
		return BoxRenderer::Vec2{ box_->bottomLeft().x + box_->position().x, box_->bottomLeft().y + box_->position().y };
	}

	void update(const float dt) {
		// values of move will be either 1 or -1 for both coordinates
		// cars only move on x and are always moving
		box_->position().x += dir_ * speed_ * dt;
			
		// when car goes offscreen, we move it to the other side of the screen
		// (like pac-man)
		// limits are fixed
		if (box_->position().x < -2) {
			box_->position().x = 1.5f;
			return;
		}
		// right
		if (box_->position().x > 2) {
			box_->position().x = -1.5f;
			return;
		}
	}
};