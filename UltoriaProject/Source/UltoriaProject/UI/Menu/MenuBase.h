// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuBase.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UMenuBase : public UUserWidget
{
	GENERATED_BODY()

public:
	// メニュー表示時関数
	virtual void OnEnter(){}

	// メニュー非表示時関数
	virtual void OnExit(){}

	// 選択ナビ移動関数
	virtual void NavigateUp(){}
	virtual void NavigateDown(){}
	virtual void NavigateLeft(){}
	virtual void NavigateRight(){}

	// LRボタン処理
	virtual void OnLButton(){}
	virtual void OnRButton(){}
	virtual void OnL2Button(){}
	virtual void OnR2Button(){}

	// 選択決定関数
	virtual void Confirm(){}
	// キャンセル決定関数
	virtual void Cancel(){}

	// 表示・非表示
	void Show();
	void Hide();
};
