# Spicavir Engine
このゲームエンジンはDirectX12を用いてレンダリングを行うゲームエンジンです。
「C++でのゲーム開発のしやすさ」「少人数チーム制作での作業分担のしやすさ」の2点を重視して設計しています。
#### 基本機能
- DirectX12による3D描画
- DxLibライクな2D描画関数群
- IPostEffectorを継承したクラスとシェーダーを作るだけの簡単なポストエフェクト
- EmitterテンプレートクラスとIParticle基底クラスにより簡単に作れるGPUパーティクル
- Assimpライブラリを用いたGLTFの読み込みとスキニングアニメーション再生
- XAudio2によるサウンド再生
- DInputとXInputによるキーボード、マウス、ゲームパッド入力
- EffekseerライブラリによるVFX読み込みと再生

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
- [Cereal](https://github.com/USCiLab/cereal)
  - データのシリアライズ・デシリアライズに使用
