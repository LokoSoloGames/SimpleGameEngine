#pragma once

#include <sgecore/math/Rect2.h>
#include <sgebase.h>

namespace SimpleGameEngine {

	class NativeUIWindowBase : public NonCopyable {
	public:

		struct CreateDesc {
			CreateDesc()
					: titleBar(true)
					, isMainWindow(false)
					, visible(true)
					, resizable(true)
					, closeButton(true)
					, minButton(true)
					, maxButton(true)
					, centerToScreen(true)
					, alwaysOnTop(false)
					, ownContext(false)
			{}

			enum class Type {
				None,
				NormalWindow,
				ToolWindow,
				PopupWindow,
			};

			Type		type = Type::NormalWindow;
			Rect2f		rect {10, 10, 640, 480};

			bool		titleBar		: 1;
			bool		isMainWindow	: 1;
			bool		visible			: 1;
			bool		resizable		: 1;
			bool		closeButton		: 1;
			bool		minButton		: 1;
			bool		maxButton		: 1;
			bool		centerToScreen	: 1;
			bool		alwaysOnTop		: 1;
			bool		ownContext		: 1;
		};

		void create			(CreateDesc& desc)	{ onCreate(desc); }
		void setWindowTitle	(StrView title)		{ onSetWindowTitle(title); }

		void drawNeeded() { onDrawNeeded(); }

		virtual void onCloseButton() {}
		virtual void onActive(bool isActive) {}
		virtual void onUpdate() {}
		virtual void onDraw() {}

	protected:
		virtual void onCreate(CreateDesc& desc) {}
		virtual void onSetWindowTitle(StrView title) {}
		virtual void onDrawNeeded() {}
	};

}