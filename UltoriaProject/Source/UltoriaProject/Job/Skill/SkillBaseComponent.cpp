// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBaseComponent.h"
#include "SkillSubsystem.h"
#include "../../Character/CharacterBase.h"
#include "../../Battle/Combat/Components/CombatComponent.h"
#include "../../Character/Data/CharacterDataManager.h"

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

void USkillBaseComponent::SetSkillData(ESkillType type, FName name, int mpCost, float effectValue, float breakMult, int unlockLevel)
{
	m_Type			= type;
	m_Name			= name;
	m_MpCost		= mpCost;
	m_EffectValue	= effectValue;
	m_BreakMult		= breakMult;
	m_UnLockLevel	= unlockLevel;
}

///// <summary>
///// スキルが発動できるか
///// </summary>
///// <param name="mp">	 現在のMP			</param>
///// <param name="mpCost">スキルに消費するMP </param>
///// <returns>true：発動できる,false：発動できない</returns>
//bool USkillBaseComponent::CanSkillInvocation(int mp)
//{
//	// MPが足りるか
//	if (mp < m_MpCost)
//	{
//		return false;
//	}
//
//	return true;
//}
//
///// <summary>
///// スキル発動
///// </summary>
///// <param name="skillId">発動させるスキルのID</param>
//void USkillBaseComponent::SkillInvocation(FString type, int skillId, int mp, int mpCost, int pAtk)
//{
//	mp -= mpCost;
//}

void USkillBaseComponent::Create(FName name)
{
	UCharacterDataManager* dataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!dataMg) return;

	FSkillDataRow skillData = dataMg->GetSkillData(name);

	if (m_SkillList.Num() < SKILL_MAX)
	{
		m_SkillList.Add(skillData);
	}
}

void USkillBaseComponent::Remove(int index)
{
	// 指定されたIndexのSkillを削除して詰める
	m_SkillList.RemoveAt(index);
}

void USkillBaseComponent::Reset()
{
	m_SkillList.Reset();
}

bool USkillBaseComponent::CanUse()
{
	if (m_Owner->GetCurrentMP() >= m_MpCost)
	{
		return true;
	}
	return false;
}

void USkillBaseComponent::UseSkill(const TArray<ACharacterBase*>& targets)
{
	if (!CanUse())
	{
		UE_LOG(LogTemp, Log, TEXT("Can Not Skill！"));
		return;
	}

	switch (m_Type)
	{
	case ESkillType::ATTACK:
		Attack(targets);
		break;
	case ESkillType::BUFF:
		Buff(targets);
		break;
	case ESkillType::HEAL:
		Heal(targets);
		break;
	}

	int newMp = m_Owner->GetCurrentMP() - m_MpCost;
	m_Owner->SetMP(newMp);
}

TArray<FSkillDataRow> USkillBaseComponent::GetUnlockedSkills()
{
	TArray<FSkillDataRow> result;

	for (const FSkillDataRow& skill : m_SkillList)
	{
		if (m_Owner->GetLevel() >= skill.UnLockLv)
		{
			result.Add(skill);
		}
	}

	return result;
}

void USkillBaseComponent::Attack(const TArray<ACharacterBase*>& targets)
{
	for (auto& target : targets)
	{
		if (!target) continue;

		UCombatComponent* conbat = m_Owner->GetComponent<UCombatComponent>();
		if (!conbat) continue;

		m_Owner->GetTotalStatus();
		conbat->Attack(target);
	}
}

void USkillBaseComponent::Buff(const TArray<ACharacterBase*>& targets)
{

}

void USkillBaseComponent::Heal(const TArray<ACharacterBase*>& targets)
{

}
