#pragma once
class HierarchyPanel final
{
public:
	void Draw();

	void OnImGuiRender();

	void ShowItemRecursive(IComponent* current);

	void DragDropTarget(IComponent* current);

	void DDTargetTexture(IComponent* current);
	void DDTargetModel(IComponent* current);

	static void SDraw();

private:

	int32_t itemIndex = 0;

public://singleton
	HierarchyPanel(const HierarchyPanel&) = delete;
	HierarchyPanel(HierarchyPanel&&) = delete;
	HierarchyPanel& operator=(const HierarchyPanel&) = delete;
	HierarchyPanel& operator=(HierarchyPanel&&) = delete;

	static HierarchyPanel* GetInstance();

private:
	HierarchyPanel() = default;
	~HierarchyPanel() = default;
};

