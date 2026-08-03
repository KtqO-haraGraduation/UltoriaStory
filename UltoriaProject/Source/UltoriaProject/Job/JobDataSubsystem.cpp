// Fill out your copyright notice in the Description page of Project Settings.


#include "JobDataSubsystem.h"
#include "Skill/SkillSubsystem.h"
#include "JobDataAsset.h"
#include "JobDataRow.h"
#include "JobLvRow.h"

#define STATUS_DIAMETER 100.0f // 倍率

UJobDataSubsystem::UJobDataSubsystem()
{
	for (int i = FENCER; i < JOB_MAX; i++)
	{
		m_JobData[i].m_Name			= "";
		m_JobData[i].m_HP			= 0.0f;
		m_JobData[i].m_MP			= 0.0f;
		m_JobData[i].m_ATK			= 0.0f;
		m_JobData[i].m_MGC			= 0.0f;
		m_JobData[i].m_DEF			= 0.0f;
		m_JobData[i].m_SPD			= 0.0f;
		m_JobData[i].m_Break		= 0.0f;
		m_JobData[i].m_BreakGauge	= 0.0f;
	}
}

// Called when the game starts
void UJobDataSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Collection.InitializeDependency<USkillSubsystem>();	// USkillSubsystemを先に初期化

	Super::Initialize(Collection);

	LoadJobDataAsset();

	if (!m_JobDA)
	{
		UE_LOG(LogTemp, Error, TEXT("JobDataAsset/false"));
		return;
	}

	// 初期化
	m_JobDataRows.Empty();
	m_JobLvRows.Empty();
	// 全データ・レベルを取得
	m_JobDA->m_JobDataMappingTable->GetAllRows<FJobDataRow>(TEXT(""), m_JobDataRows);
	m_JobDA->m_JobLvMappingTable->GetAllRows<FJobLvRow>(TEXT(""), m_JobLvRows);

	for (int i = FENCER; i < JOB_MAX; i++)
	{
		m_JobBase[i] = CreateJobBase(i);
		// ジョブデータの要素数を超えたか
		// falseが呼ばれると超えている
		if (!m_JobDataRows.IsValidIndex(i))
		{
			continue; // 超えていたら処理をスルー
		}
		m_JobData[i] = CreateJobData(i);
	}
	
	for (int j = 0; j < JOB_LEVEL_MAX; j++)
	{
		m_TotalExp[j] = m_JobLvRows[j]->TotalExp;
	}
}

void UJobDataSubsystem::LoadJobDataAsset()
{
	m_JobDA = LoadObject<UJobDataAsset>(nullptr, TEXT("/Script/UltoriaProject.JobDataAsset'/Game/MembarFiles/Sakumoto/DataAsset/DA_JobData.DA_JobData'"));
}

FJobBase UJobDataSubsystem::CreateJobBase(int id)
{
	UGameInstance* gi = GetOuterUGameInstance();

	USkillSubsystem* skillSubsystem = gi->GetSubsystem<USkillSubsystem>();
	FJobBase jobBase;

	for (int i = 0; i < 6; i++)
	{
		jobBase.m_SkillData[i] = skillSubsystem->GetSkillData(id, i);
	}

	jobBase.m_Lv	= 0;
	jobBase.m_Exp	= 0;

	return jobBase;
}

// ジョブデータを生成
FJobData UJobDataSubsystem::CreateJobData(int id)
{
	FJobData jobData;

	jobData.m_Name	= m_JobDataRows[id]->Name;
	jobData.m_HP	= m_JobDataRows[id]->HP  / STATUS_DIAMETER;
	jobData.m_MP	= m_JobDataRows[id]->MP  / STATUS_DIAMETER;
	jobData.m_ATK	= m_JobDataRows[id]->ATK / STATUS_DIAMETER;
	jobData.m_MGC	= m_JobDataRows[id]->MGC / STATUS_DIAMETER;
	jobData.m_DEF	= m_JobDataRows[id]->DEF / STATUS_DIAMETER;
	jobData.m_SPD	= m_JobDataRows[id]->SPD / STATUS_DIAMETER;

	return jobData;
}