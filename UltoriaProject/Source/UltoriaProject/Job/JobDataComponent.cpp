// Fill out your copyright notice in the Description page of Project Settings.

#include "JobDataComponent.h"
#include "JobDataSubsystem.h"
#include "Skill/SkillSubsystem.h"
#include "Skill/SkillBaseComponent.h"
#include "../Character/CharacterBase.h"
#include "../Character/Data/CharacterDataManager.h"

#define STATUS_DIAMETER 100.0f // 倍率

// Sets default values for this component's properties
UJobDataComponent::UJobDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	m_Level = 0;
	m_Exp = 0;
	m_Break = 0.0f;
	m_BreakGauge = 0.0f;
}

void UJobDataComponent::BeginPlay()
{
    Super::BeginPlay();

	m_Level = 1;
	m_Skill = GetOwner()->FindComponentByClass<USkillBaseComponent>();
}

/// <summary>
/// ジョブデータをSet
/// </summary>
/// <param name="id">ジョブID</param>
void UJobDataComponent::SetJobData(int id, ACharacterBase* character)
{
	/*UJobDataSubsystem* jobDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UJobDataSubsystem>();
	USkillSubsystem* skillDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<USkillSubsystem>();
	if (!jobDataSubsystem || !skillDataSubsystem)
	{
		return;
	}
	const FJobData& jobData = jobDataSubsystem->GetJobData(id);

	m_HP = jobData.m_HP;
	m_MP = jobData.m_MP;
	m_ATK = jobData.m_ATK;
	m_MGC = jobData.m_MGC;
	m_DEF = jobData.m_DEF;
	m_SPD = jobData.m_SPD;
	m_Break = jobData.m_Break;
	m_BreakGauge = jobData.m_BreakGauge;

	for (int i = 0; i < SKILL_MAX; i++)
	{
		m_Skill[i] = character->FindComponentByClass<USkillBaseComponent>();
		if (!m_Skill[i])
		{
			continue;
		}
		const FSkillData& skillData = skillDataSubsystem->GetSkillData(id, i);

		m_Skill[i]->SetSkillData(skillData.m_Type, skillData.m_Name, skillData.m_MpCost, skillData.m_EffectValue, skillData.m_BreakMult, skillData.m_UnLockLv);
	}*/
}

void UJobDataComponent::SetJobData(FName jobName)
{
	UCharacterDataManager* dataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!dataMg) return;

	m_Data = dataMg->GetJobData(jobName);

	/*UE_LOG(LogTemp, Warning, TEXT("Data"));
	UE_LOG(LogTemp, Warning, TEXT("Name : %s"), *m_Data.Name.ToString());
	UE_LOG(LogTemp, Warning, TEXT("HP : %f"), m_Data.HP);
	UE_LOG(LogTemp, Warning, TEXT("MP : %f"), m_Data.MP);
	UE_LOG(LogTemp, Warning, TEXT("ATK : %f"), m_Data.ATK);
	UE_LOG(LogTemp, Warning, TEXT("MGC : %f"), m_Data.MGC);
	UE_LOG(LogTemp, Warning, TEXT("DEF : %f"), m_Data.DEF);
	UE_LOG(LogTemp, Warning, TEXT("SPD : %f"), m_Data.SPD);
	UE_LOG(LogTemp, Warning, TEXT("Skill_1 : %s"), *m_Data.Skill_1.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Skill_2 : %s"), *m_Data.Skill_2.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Skill_3 : %s"), *m_Data.Skill_3.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Skill_4 : %s"), *m_Data.Skill_4.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Skill_5 : %s"), *m_Data.Skill_5.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Skill_6 : %s"), *m_Data.Skill_6.ToString());*/

	m_Data.HP /= STATUS_DIAMETER;
	m_Data.MP /= STATUS_DIAMETER;
	m_Data.ATK /= STATUS_DIAMETER;
	m_Data.MGC /= STATUS_DIAMETER;
	m_Data.DEF /= STATUS_DIAMETER;
	m_Data.SPD /= STATUS_DIAMETER;

	/*UE_LOG(LogTemp, Warning, TEXT("HP : %f"), m_Data.HP);
	UE_LOG(LogTemp, Warning, TEXT("MP : %f"), m_Data.MP);
	UE_LOG(LogTemp, Warning, TEXT("ATK : %f"), m_Data.ATK);
	UE_LOG(LogTemp, Warning, TEXT("MGC : %f"), m_Data.MGC);
	UE_LOG(LogTemp, Warning, TEXT("DEF : %f"), m_Data.DEF);
	UE_LOG(LogTemp, Warning, TEXT("SPD : %f"), m_Data.SPD);*/

	CreateSkill();
}

bool UJobDataComponent::CanLevelUp()
{
	UCharacterDataManager* dataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!dataMg) return false;

	int nexJobLv = m_Level + 1;
	FJobLvRow jobLvData = dataMg->GetJobLvData(nexJobLv);

	return m_Exp >= jobLvData.TotalExp;
}

void UJobDataComponent::LevelUp()
{
	if (!CanLevelUp()) return;
	m_Level++;
}

void UJobDataComponent::CreateSkill()
{
	m_Skill->Reset();

	for (FName skill : m_Data.Skills)
	{
		m_Skill->Create(skill);
	}
}

