#include "Skydome.h"
#include "ImGuiManager.h"
#include <cassert>

void Skydome::Initialize(Model* model) {
	// ヌルじゃないか確認
	assert(model);
	// 外から貰ってきたデータの受け渡し
	model_ = model;
	//  ワールドトランスフォーム初期化(プレイヤーに移動するカメラ個体ごと)
	worldTransform_.Initialize();
}

void Skydome::Update() 
{

}

void Skydome::Draw(ViewProjection viewprojection) { 
	
	model_->Draw(worldTransform_, viewprojection);

}
