// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillBaseComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API USkillBaseComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	FString GetSkillType() { return m_Type; }
	FString GetName() { return m_Name; }
	int GetMpCost() { return m_MpCost; }
	float GetEffectValue() { return m_EffectValue; }
	float GetBreakMult() { return m_BreakMult; }
	int GetUnLockLevel() { return m_UnLockLevel; }
	int GetDuration() { return m_Duration; }

	void SetSkillData(FString type, FString name, int mpCost, float effectValue, float breakMult, int unlockLevel);

public:	
	bool CanSkillInvocation(int mp);										// スキルが発動できるか
	void SkillInvocation(FString type, int skillId, int mp, int mpCost, int pAtk);	// スキル発動

private:
	FString m_Type;		// スキルタイプ
	FString  m_Name;	// 技名
	int   m_MpCost;		// 消費MP
	float m_EffectValue;// 効果倍率
	float m_BreakMult;	// ブレイク倍率
	int m_UnLockLevel;	// 解除レベル
	int m_Duration;		// 効果時間
};
