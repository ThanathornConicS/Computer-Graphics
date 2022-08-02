#pragma once

#include <pch.h>

namespace vlr
{
	class Input
	{
	private:
		static Input* s_instance;

	protected:
		virtual bool IsKeyPressedImpl(int keycode) = 0;
		virtual bool IsKeyReleasedImpl(int keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<double, double> GetMousePositionImpl() = 0;
		virtual double GetMouseXImpl() = 0;
		virtual double GetMouseYImpl() = 0;

	public:
		inline static bool IsKeyPressed(int keycode) { return s_instance->IsKeyPressedImpl(keycode); }
		inline static bool IsKeyReleased(int keycode) { return s_instance->IsKeyReleasedImpl(keycode); }

		inline static bool IsMouseButtonPressed(int button) { return s_instance->IsMouseButtonPressedImpl(button); }
		inline static std::pair<double, double> GetMousePosition() { return s_instance->GetMousePositionImpl(); }
		inline static double GetMouseX() { return s_instance->GetMouseXImpl(); }
		inline static double GetMouseY() { return s_instance->GetMouseYImpl(); }

	};
}