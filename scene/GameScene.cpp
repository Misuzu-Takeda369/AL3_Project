#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// スプライト用
	textureHandle1_ = TextureManager::Load("sample.png");
	sprite_ = Sprite::Create(textureHandle1_, {100, 50});
	// モデル用
	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();

	// 音楽
	soundDatehandle_ = audio_->LoadWave("fanfare.wav");
	audio_->PlayWave(soundDatehandle_, true);

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// ライン描画が参照するビュープロジェクション指定
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	// カメラ生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 軸
	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {

	// はじめと終わりを着けることでウィンドウを名づけられる
	ImGui::Begin("Debug1");
	// ImGui::Text("Kamata Tarou %d.%d.%d", 2050,12,31);
	//
	ImGui::InputFloat3("InputFloat3", inputFloat3);

	ImGui::SliderFloat3("InputFloat3", inputFloat3, 0.0f, 1.0f);

	ImGui::End();

	ImGui::ShowDemoWindow();

	debugCamera_->Update();

	// ループを止める
	if (input_->TriggerKey(DIK_SPACE)) {

		audio_->StopWave(soundDatehandle_);
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	sprite_->Draw();
	///
	/// //ラインを描画する
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
