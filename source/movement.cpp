#include <box_renderer.h>

// manages player movement
class Movement {
private:
	BoxRenderer::BoxId box_id_;
	bool moving_ = false;
	BoxRenderer::Vec2 move_ = { 0,0 };

public:
	Movement(BoxRenderer::BoxId& id) {
		box_id_ = id;
	}

	BoxRenderer::BoxId& id() {
		return box_id_;
	}

	const BoxRenderer::BoxId& id() const {
		return box_id_;
	}

	void set_move(BoxRenderer::Vec2 move) {
		move_ = move;
	}

	void update(float dt) {
		return;
	}
};