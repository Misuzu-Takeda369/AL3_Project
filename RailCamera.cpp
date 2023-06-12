#include "RailCamera.h"
#include "ImGuiManager.h"

void RailCamera::Initialize(Vector3 trans, Vector3 rot) { 

	//ワールドトランスフォームの引数
	worldTransform_.translation_ = trans;
	worldTransform_.rotation_ = rot;

	//モデルを描写するわけではないのでワールドトランスフォームの初期化を書かなくて良い。

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

#pragma region 移動処理
	//アフェインに全部まとめるか(次がworldTransform_.matWorldなのでその辺?)
	Vector3 move = {0.0f,0.0f,0.0f};
	Vector3 rot = {0.0f,0.0f,0.0f};

	
	// 移動
	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;
	
	//角度
	worldTransform_.rotation_.x += rot.x;
	worldTransform_.rotation_.y += rot.y;
	worldTransform_.rotation_.z += rot.z;

#pragma endregion

	// 行列更新(ここが動かない原因知らん)
	worldTransform_.UpdateMatrix();
	//ビュープロジェクションは逆行列
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);
}

void RailCamera::Draw()
{ 
	//model_->Draw(worldTransform_, viewProjection_);
}
