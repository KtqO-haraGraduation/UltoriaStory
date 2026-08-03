// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "MenuStateManager.h"
#include "Components/Border.h"
#include "MenuMain.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UMenuMain : public UMenuBase
{
	GENERATED_BODY()

public:
	// 開始時処理と終了時処理関数
	virtual void OnEnter() override;
	virtual void OnExit() override;

	// 選択ナビ移動関数
	virtual void NavigateUp() override;
	virtual void NavigateDown() override;
	// 選択決定処理
	virtual void Confirm() override;

private:
	// 選択更新関数
	void UpdateSelection_Menu();

private:
	// 選択番号
	int32 m_SelectedIndex = 0;
	// 最大番号
	int32 m_MaxIndex = 5;

	// 各メニュー選択ハイライトリスト
	TArray<UBorder*> m_Borders;

	// 各メニュー選択ハイライト
	UPROPERTY(meta = (BindWidget))
	UBorder* m_BorderStates;
	UPROPERTY(meta = (BindWidget))
	UBorder* m_BorderItems;
	UPROPERTY(meta = (BindWidget))
	UBorder* m_BorderQuest;
	UPROPERTY(meta = (BindWidget))
	UBorder* m_BorderOptions;
	UPROPERTY(meta = (BindWidget))
	UBorder* m_BorderSave;
	UPROPERTY(meta = (BindWidget))
	UBorder* m_BorderEnd;
};
