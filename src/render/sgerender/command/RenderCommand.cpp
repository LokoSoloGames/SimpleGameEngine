#include "RenderCommand.h"

namespace SimpleGameEngine {
	void RenderCommandBuffer::reset() {
		for (auto* cmd : _commands) {
			cmd->~RenderCommand();
		}
		_commands.clear();
		_allocator.clear();
	}
}