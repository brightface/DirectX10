#pragma once
#include "stdafx.h"

//Singletone Pattern
class Settings final
{
public:
	static Settings& Get()
	{
		static Settings instance;
		return instance;
	}

	auto GetWindowHandle() const -> HWND { return handle; }
	void SetWindowHandle(HWND handle) { this->handle = handle; }

	auto GetWidth() const -> const float& { return width; }
	void SetWidth(const float& width) { this->width = width; }

	auto GetHeight() const -> const float& { return height; }
	void SetHeight(const float& height) { this->height = height; }

private:
	Settings() = default;
	~Settings() = default;

private:
	HWND handle = nullptr;
	float width = 0.0f;
	float height = 0.0f;
};