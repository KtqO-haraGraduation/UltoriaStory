// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIBase.h"
#include "CommandCursorWidget.generated.h"

enum class CommandCursorState
{
	IDLE,		// 待機中
	MOVE_IN,	// 出現途中（通常再生）
	MOVE_OUT,	// 消える途中（逆再生）
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCommandCursorWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	// アニメーション再生関数
	void PlayIdle();
	void StopIdle();

	void PlayMove();
	void ReverseMove();

	// カーソルの座標を設定
	void SetCursorPos(const FVector2D& pos);

	// カーソルを指定した位置に移動
	void MoveTo(const FVector2D& pos);

	void SetFirst(bool flag) { m_IsFirst = flag; }

protected:
	virtual void NativeConstruct() override;

private:
	UFUNCTION()
	void OnMoveAnimFinished();

private:
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* IdleAnim;

	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* MoveAnim;

	bool m_IsFirst = true;
	FVector2D m_NextPos;
	CommandCursorState m_State;
};
