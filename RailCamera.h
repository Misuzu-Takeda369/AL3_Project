#pragma once
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCamera {
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
	const ViewProjection& GetViewProjection() { return viewProjection_; }
	const WorldTransform& GetWorldProjection() { return worldTransform_; };

	/// <summary>
	/// ワールド行列に変換する関数
	/// </summary>
	/// <returns></returns>
	//Vector3 GetWorldPosition();

private:
	/// ワールド変換データ
	WorldTransform worldTransform_;

	/// ビュープロジェクション(この子は普段カメラに渡していたが今回はこの子自体がカメラなのでここに作成)
	ViewProjection viewProjection_;
};
