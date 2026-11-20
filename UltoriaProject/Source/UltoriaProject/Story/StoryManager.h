// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StoryManager.generated.h"

class UObjectiveWidget;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UStoryManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;

	// 現在のストーリの進行度に合わせたイベントを取得する
	UDataTable* TryGetEvent(FName triggerID, FName& outRowName);
	
	// イベントの終了を確認する
	void CompleteEvent(FName rowName);

	// 目的表示のWidgetを作る
	void CreateObjectiveWidget(UWorld* world);

	// 目的表示の表示/非表示関数
	void SetObjectiveVisble(bool isVisible);

	// 現在の行取得
	FName GetCurrentRow() const {return m_CurrentRow; }
	// 現在のストーリー行のトリガー取得
	FName GetCurrentTriggerID() const;


private:
	void UpdateObjective();

private:
	// ストーリのデータテーブル
	UPROPERTY()
	UDataTable* m_StoryTable = nullptr;

	// 今の行名
	FName m_CurrentRow;

	// 目的表示用テキスト
	UPROPERTY()
	UObjectiveWidget* m_ObjectiveWidget = nullptr;

	// 目的表示/非表示フラグ
	bool m_IsObjectiveVisible = true;
};
