// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBaseComponent.h"
#include "SkillSubsystem.h"
#include "../../Character/CharacterBase.h"
#include "../../Battle/Combat/Components/CombatComponent.h"
#include "../../Character/Data/CharacterDataManager.h"

// 回復時の乱数倍率
constexpr float HEAL_RANDOM_RATE_MIN = 0.9f;
constexpr float HEAL_RANDOM_RATE_MAX = 1.1f;

// Sets default values for this component's properties
USkillBaseComponent::USkillBaseComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void USkillBaseComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

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

bool USkillBaseComponent::CanUse(int index)
{
	const FSkillDataRow* skill = GetSkill(index);
	if (!skill) return false;

	if (m_Owner->GetCurrentMP() >= skill->MpCost)
	{
		return true;
	}
	return false;
}

void USkillBaseComponent::UseSkill(int index, const TArray<ACharacterBase*>& targets)
{
	m_NowUseSkillData = GetSkill(index);
	if (!m_NowUseSkillData) return;

	if (!CanUse(index))
	{
		// MPが足りないから使用できないをTextで表示するようにする

		UE_LOG(LogTemp, Log, TEXT("Can Not Skill！"));
		return;
	}

	switch (m_NowUseSkillData->SkillType)
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

	int newMp = m_Owner->GetCurrentMP() - m_NowUseSkillData->MpCost;
	m_Owner->SetMP(newMp);
}

bool USkillBaseComponent::IsUnlocked(int index)
{
	if (m_Owner->GetLevel() >= m_SkillList[index].UnLockLv)
	{
		return true;
	}
	return false;
}

const FSkillDataRow* USkillBaseComponent::GetSkill(int index) const
{
	if (!m_SkillList.IsValidIndex(index))
	{
		return nullptr;
	}

	return &m_SkillList[index];
}

TArray<FSkillDataRow> USkillBaseComponent::GetUnlockedSkills()
{
	TArray<FSkillDataRow> result;

	for (int i = 0; i < SKILL_MAX; i++)
	{
		if (IsUnlocked(i))
		{
			result.Add(m_SkillList[i]);
		}
	}
	return result;
}

void USkillBaseComponent::Attack(const TArray<ACharacterBase*>& targets)
{
	UCombatComponent* conbat = m_Owner->GetComponent<UCombatComponent>();
	if (!conbat) return;
	for (auto& target : targets)
	{
		if (!target) continue;
		conbat->Attack(target);
	}
	conbat->NextTurn();
}

void USkillBaseComponent::Buff(const TArray<ACharacterBase*>& targets)
{



}

void USkillBaseComponent::Heal(const TArray<ACharacterBase*>& targets)
{
	for (auto& target : targets)
	{
		if (!target) continue;

		UCombatComponent* conbat = target->GetComponent<UCombatComponent>();
		if (!conbat) continue;

		float randomRate = FMath::FRandRange(HEAL_RANDOM_RATE_MIN, HEAL_RANDOM_RATE_MAX);
		int healHP = m_NowUseSkillData->EffectValue * randomRate;

		conbat->HPHeal(healHP);
	}
}
