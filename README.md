# Spicavir Engine
このゲームエンジンはDirectX12を用いてレンダリングを行うゲームエンジンです。
「C++でのゲーム開発のしやすさ」「少人数チーム制作での作業分担のしやすさ」の2点を重視して設計しています。
#### 基本機能
- DirectX12による3D描画
- DxLibライクな2D描画関数群
- DrawLine関数による当たり判定やエミッターのデバッグ機能
- IPostEffectorを継承したクラスとシェーダーを作るだけの簡単なポストエフェクト
- EmitterテンプレートクラスとIParticle基底クラスにより簡単に作れるパーティクルシステム
- Assimpライブラリを用いたGLTFの読み込みとスキニングアニメーション再生
- XAudio2によるサウンド再生
- DInputとXInputによるキーボード、マウス、ゲームパッド入力
- EffekseerライブラリによるVFX読み込みと再生
- 描画順を自動で決定することによる半透明オブジェクトのより正確な描画
  
#### リファレンス(WIP)
[リファレンス](https://scrapbox.io/SpicavirEngine/Spicavir_Engine_%E7%B0%A1%E6%98%93%E3%83%AA%E3%83%95%E3%82%A1%E3%83%AC%E3%83%B3%E3%82%B9)

#### 追加予定の機能
- GPUパーティクル
- リソースのシリアライズ
- 調整したパラメータの保存・読み込み

#### 使用外部ライブラリ
- [Assimp](https://github.com/assimp/assimp)
  - ボーン・アニメーション付きモデルの読み込みに使用
- [DirectXTex](https://github.com/microsoft/DirectXTex)
  - テクスチャの読み込みに使用
- [EASTL](https://github.com/electronicarts/EASTL)  
  - STLコンテナ系の軽量化に使用
- [Effekseer](https://effekseer.github.io/jp/)
  - Effekseerで制作したVFXの読み込みと再生に使用
- [Dear ImGui](https://github.com/ocornut/imgui)
  - インタラクティブなデバッグ用GUIに使用
