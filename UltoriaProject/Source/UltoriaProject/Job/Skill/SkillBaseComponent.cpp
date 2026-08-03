// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBaseComponent.h"
#include "SkillSubsystem.h"

// Sets default values for this component's properties
USkillBaseComponent::USkillBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	m_Name = "";
	m_MpCost = 0;
	m_EffectValue = 0.0f;
	m_BreakMult = 0.0f;
	m_UnLockLevel = 0;
}


// Called when the game starts
void USkillBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void USkillBaseComponent::SetSkillData(FString type, FString name, int mpCost, float effectValue, float breakMult, int unlockLevel)
{
	m_Type			= type;
	m_Name			= name;
	m_MpCost		= mpCost;
	m_EffectValue	= effectValue;
	m_BreakMult		= breakMult;
	m_UnLockLevel	= unlockLevel;
}

/// <summary>
/// スキルが発動できるか
/// </summary>
/// <param name="mp">	 現在のMP			</param>
/// <param name="mpCost">スキルに消費するMP </param>
/// <returns>true：発動できる,false：発動できない</returns>
bool USkillBaseComponent::CanSkillInvocation(int mp)
{
	// MPが足りるか
	if (mp < m_MpCost)
	{
		return false;
	}

	return true;
}

/// <summary>
/// スキル発動
/// </summary>
/// <param name="skillId">発動させるスキルのID</param>
void USkillBaseComponent::SkillInvocation(FString type, int skillId, int mp, int mpCost, int pAtk)
{
	mp -= mpCost;
}