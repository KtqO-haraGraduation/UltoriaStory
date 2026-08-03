// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkillParameter.h"
#include "../JobParameter.h"
#include "SkillSubsystem.generated.h"

struct FSkillDataRow;
class USkillDataAsset;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API USkillSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	USkillSubsystem();

protected:
	// Called when the game starts
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadSkillDataAsset();	// データアセットをロード

	virtual void Deinitialize() {};

public:
	FSkillData* GetSkillData(int jobId, int skillID) { return &m_SkillData[jobId][skillID]; }	// スキルデータを取得

private:
	FSkillData CreateSkillData(int id);	// スキルデータを生成

private:
	FSkillData m_SkillData[JOB_MAX][SKILL_MAX];	// スキルの構造体[ジョブID][スキルID]

	// データテーブルを呼び出すためのデータアセット
	USkillDataAsset* m_SkillDA;
	// 全スキルデータのリスト
	TArray<FSkillDataRow*> m_Rows;
};
