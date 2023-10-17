#pragma once
class InspectorWindow
{
public:
	static void SelectObject(IComponent* component);

	void DrawWindow();

	static void SDraw();

private:
	IComponent* selected_ = nullptr;

public://singleton
	InspectorWindow(const InspectorWindow&) = delete;
	InspectorWindow(InspectorWindow&&) = delete;
	InspectorWindow& operator=(const InspectorWindow&) = delete;
	InspectorWindow& operator=(InspectorWindow&&) = delete;

	static InspectorWindow* GetInstance();

private:
	InspectorWindow() = default;
	~InspectorWindow() = default;
};

