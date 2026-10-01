// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Battle/Combat/Components/ComponentBase.h"
#include "SkillDataRow.h"
#include "SkillBaseComponent.generated.h"

class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API USkillBaseComponent : public UComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	ESkillType GetSkillType() { return m_Type; }
	FName GetName() { return m_Name; }
	int GetMpCost() { return m_MpCost; }
	float GetEffectValue() { return m_EffectValue; }
	float GetBreakMult() { return m_BreakMult; }
	int GetUnLockLevel() { return m_UnLockLevel; }
	int GetDuration() { return m_Duration; }

	void SetSkillData(ESkillType type, FName name, int mpCost, float effectValue, float breakMult, int unlockLevel);

public:	
	//bool CanSkillInvocation(int mp);										// スキルが発動できるか
	//void SkillInvocation(FString type, int skillId, int mp, int mpCost, int pAtk);	// スキル発動

	void Create(FName name);
	void Remove(int index);
	void Reset();

	bool CanUse();
	void UseSkill(const TArray<ACharacterBase*>& targets);

	TArray<FSkillDataRow> GetUnlockedSkills();

private:
	// 各スキル関数
	void Attack(const TArray<ACharacterBase*>& targets);
	void Buff(const TArray<ACharacterBase*>& targets);
	void Heal(const TArray<ACharacterBase*>& targets);

private:
	ESkillType m_Type;		// スキルタイプ
	FName  m_Name;	// 技名
	int   m_MpCost;		// 消費MP
	float m_EffectValue;// 効果倍率
	float m_BreakMult;	// ブレイク倍率
	int m_UnLockLevel;	// 解除レベル
	int m_Duration;		// 効果時間

	TArray<FSkillDataRow> m_SkillList;
};
