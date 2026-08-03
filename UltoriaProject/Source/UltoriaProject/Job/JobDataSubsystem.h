// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "JobParameter.h"
#include "JobDataSubsystem.generated.h"

struct FJobDataRow;
struct FJobLvRow;
class UJobDataAsset;

UCLASS()
class ULTORIAPROJECT_API UJobDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// Sets default values for this component's properties
	UJobDataSubsystem();

protected:
	// Called when the game starts
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	void LoadJobDataAsset();

	virtual void Deinitialize() {};

public:
	FJobBase* GetJobBase(int id) { return &m_JobBase[id]; }
	FJobData* GetJobData(int id) { return &m_JobData[id]; }
	int GetTotalExp(int lv) { return m_TotalExp[lv]; }

private:
	FJobBase CreateJobBase(int id);
	FJobData CreateJobData(int id);

private:
	FJobBase m_JobBase[JOB_MAX];
	FJobData m_JobData[JOB_MAX];
	int m_TotalExp[JOB_LEVEL_MAX];

	// データテーブルを呼び出すためのデータアセット
	UJobDataAsset* m_JobDA;
	// 全ジョブデータのリスト
	TArray<FJobDataRow*> m_JobDataRows;
	// 全ジョブレベルのリスト
	TArray<FJobLvRow*>	 m_JobLvRows;
};
