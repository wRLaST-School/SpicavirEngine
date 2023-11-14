#include "stdafx.h"
#include "DockPanel.h"
#include <GameManager.h>
#include <SpTextureManager.h>
#include <Util.h>
#include <InspectorWindow.h>
#include <Object3D.h>
#include <SceneManager.h>
#include <SceneRW.h>

void DockPanel::EnableScreenDock()
{
	SpImGui::Command([&] {
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		if (!GameManager::sShowDebug) dockspace_flags |= ImGuiDockNodeFlags_::ImGuiDockNodeFlags_KeepAliveOnly;

		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		window_flags |= ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

		static bool open = true;

		ImGui::Begin("Editor", &open, window_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save"))
				{
					ImGui::OpenPopup("SaveScenePopup");
				}

				//コンポーネント選択ポップアップの設定
				//TODO:ダイアログにする
				if (ImGui::BeginPopupModal("SaveScenePopup", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
				{
					const int PATH_LENGTH = 256;
					static char path[PATH_LENGTH];

					ImGui::InputText("FileName", path, PATH_LENGTH);

					if (ImGui::Button("Save"))
					{
						SceneRW::SaveScene(SceneManager::currentScene.get(), std::string("Assets/Scene/") + std::string(path) + std::string(".scene"));
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}

				if (ImGui::MenuItem("Load"))
				{
					SceneRW::LoadScene(SceneManager::currentScene.get(), "Assets/Scene/TestSceneSave.scene");
				}

				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}


		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		ImGui::End();

		ImGui::PopStyleVar(3);
		});
}

void DockPanel::DrawViewPort()
{
	if (GameManager::sShowDebug)
	{
		SpImGui::Command([] {
			static bool open = true;
			ImGui::Begin("Game", &open, ImGuiWindowFlags_NoCollapse);

			viewPortWindow = ImGui::GetCurrentWindow();

			//タブ等を除いたウィンドウのサイズを取得(計算)
			ImVec2 cntRegionMax = ImGui::GetWindowContentRegionMax();
			ImVec2 cntRegionMin = ImGui::GetWindowContentRegionMin();
			ImVec2 wndSize = {cntRegionMax.x - cntRegionMin.x, cntRegionMax.y - cntRegionMin.y};
	
			//元のアス比とImGuiウィンドウのアス比を比較
			float outerWindowAspectRatio = static_cast<float>(Util::GetWinWidth()) / static_cast<float>(Util::GetWinHeight());
			float innerWindowAspectRatio = wndSize.x / wndSize.y;
			ImVec2 finalWndSize = wndSize;

			//横幅が大きかったら縦基準で画像サイズを決定
			if (outerWindowAspectRatio <= innerWindowAspectRatio)
			{
				finalWndSize.x *= outerWindowAspectRatio / innerWindowAspectRatio ;
			}
			//縦幅が大きかったら横基準で画像サイズを決定
			else
			{
				finalWndSize.y *= innerWindowAspectRatio / outerWindowAspectRatio;
			}

			//画像を中央に持ってくる
			ImVec2 topLeft = { (wndSize.x - finalWndSize.x) * 0.5f + cntRegionMin.x,
								(wndSize.y - finalWndSize.y) * 0.5f + cntRegionMin.y };
			ImGui::SetCursorPos(topLeft);

			ImGui::Image((ImTextureID)SpTextureManager::GetGPUDescHandle("RenderTexture").ptr,
				finalWndSize);

			//Gizmo対応型だったらGizmoを表示
			Object3D* selectedObj = InspectorWindow::GetSelected<Object3D>();
			if (selectedObj)
			{
				selectedObj->DrawGizmo();
			}

			ImGui::End();
		});
	}
}

ImGuiWindow* DockPanel::GetViewPortWindow()
{
	return viewPortWindow;
}
