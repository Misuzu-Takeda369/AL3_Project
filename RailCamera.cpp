#include "RailCamera.h"
#include "calc.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Model* model) { 

	// ヌルじゃないか確認
	assert(model);
	// 外から貰ってきたデータの受け渡し
	model_ = model;
	//ワールドトランスフォームの引数
	worldTransform_ .translation_= {0.0f,0.0f,0.0f};
	worldTransform_.rotation_ = {0.0f,0.0f,0.0f};

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() 
{

#pragma region ImGui
	// 窓
	ImGui::SetNextWindowPos({0, 0});
	ImGui::SetNextWindowSize({300, 100});
	// プレイヤー座標表示
	ImGui::Begin("Camera");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};

	float sliderValue1[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z};
	ImGui::SliderFloat3("rotation_", sliderValue1, -20.0f, 20.0f);
	worldTransform_.rotation_ = {sliderValue1[0], sliderValue1[1], sliderValue1[2]};
	ImGui::End();

#pragma endregion

	//アフェインに全部まとめるか(次がworldTransform_.matWorldなのでその辺?)
	worldTransform_.UpdateMatrix();
	//ビュープロジェクションは逆行列
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
}

void RailCamera::Draw() 
{

}
