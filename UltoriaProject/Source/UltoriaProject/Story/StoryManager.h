// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StoryManager.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UStoryManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection)override;


	UDataTable* TryGetEvent(FName triggerID, FName& outRowName);
	
	void CompleteEvent(FName rowName);

	FName GetCurrentRow() const {return m_CurrentRow; }

private:
	// ストーリのデータテーブル
	UPROPERTY()
	UDataTable* m_StoryTable = nullptr;

	// 今の行名
	FName m_CurrentRow;
};
