#pragma once

#include "EditorWindow.h"

namespace SimpleGameEngine {

class EditorHierarchyWindow : public EditorWindow {
public:

	void draw(RenderRequest& req, Scene& scene);

private:
	bool _active = true;
};

}