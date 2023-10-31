#pragma once
class ResourceWindow
{
public:
	void Draw();

	static void SDraw();

private:
	float thumbnailSize = 128.f;
	float padding = 16.f;

public://singleton
	ResourceWindow(const ResourceWindow&) = delete;
	ResourceWindow(ResourceWindow&&) = delete;
	ResourceWindow& operator=(const ResourceWindow&) = delete;
	ResourceWindow& operator=(ResourceWindow&&) = delete;

	static ResourceWindow* GetInstance();

private:
	ResourceWindow() = default;
	~ResourceWindow() = default;
};

