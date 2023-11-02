#pragma once
class InspectorWindow
{
public:
	static void SelectObject(IComponent* component);

	void DrawWindow();

	template <class Type>
	static Type* GetSelected();

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

template<class Type>
inline Type* InspectorWindow::GetSelected()
{
	return dynamic_cast<Type*>(GetInstance()->selected_);
}