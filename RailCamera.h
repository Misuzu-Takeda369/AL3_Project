#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera 
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3 trans, Vector3 rot);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描写
	/// </summary>
	void Draw();

	/// <summary>
	/// ビュープロジェクションを取得(レールカメラにも)
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	ViewProjection& GetViewProjection() { return viewProjection_; }

	//ViewProjection& GetViewPro

private:

	/// ワールド変換データ
	WorldTransform worldTransform_;

	/// ビュープロジェクション(この子は普段カメラに渡していたが今回はこの子自体がカメラなのでここに作成)
	ViewProjection viewProjection_;


};
