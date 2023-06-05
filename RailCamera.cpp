#include "RailCamera.h"
#include "calc.h"

void RailCamera::Initialize(WorldTransform worldTransform, ViewProjection viewProjection) { 
	//ワールドトランスフォームの引数
	worldTransform_ .translation_= {0.0f,0.0f,0.0f};
	worldTransform_.rotation_ = {};
}

void RailCamera::Update() 
{

}

void RailCamera::Draw() 
{

}
