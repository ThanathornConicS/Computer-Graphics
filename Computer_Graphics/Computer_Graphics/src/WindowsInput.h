#pragma once

#include "Input.h"

namespace vlr
{
	class WindowsInput : public Input
	{
	protected:
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsKeyReleasedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<double, double> GetMousePositionImpl() override;
		virtual double GetMouseXImpl() override;
		virtual double GetMouseYImpl() override;
	};

}
