// Fill out your copyright notice in the Description page of Project Settings.


#include "JobDataComponent.h"
#include "JobDataSubsystem.h"
#include "Skill/SkillSubsystem.h"
#include "Skill/SkillBaseComponent.h"
#include "../Character/CharacterBase.h"

// Sets default values for this component's properties
UJobDataComponent::UJobDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	for (int i = 0; i < SKILL_MAX; i++)
	{
		m_Skill[i] = nullptr;
	}

	m_Level = 0;
	m_Exp = 0;
	m_HP = 0.0f;
	m_MP = 0.0f;
	m_ATK = 0.0f;
	m_MGC = 0.0f;
	m_DEF = 0.0f;
	m_SPD = 0.0f;
	m_Break = 0.0f;
	m_BreakGauge = 0.0f;
}

void UJobDataComponent::BeginPlay()
{
    Super::BeginPlay();

	m_Level = 1;

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

void UJobDataComponent::LevelUp()
{
	m_Level++;
}
