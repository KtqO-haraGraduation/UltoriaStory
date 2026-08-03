// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JobDataComponent.generated.h"

class ACharacterBase;
class USkillBaseComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API UJobDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJobDataComponent();

public:
	// 
	void BeginPlay() override;

public:
	USkillBaseComponent* GetSkill(int id) { return m_Skill[id]; }
	const FString GetFString()	const { return m_Name; }
	const int	GetLevel()			const { return m_Level; }
	const int	GetExp()			const { return m_Exp; }
	const float GetHP()			const { return m_HP; }
	const float GetMP()			const { return m_MP; }
	const float GetATK()		const { return m_ATK; }
	const float GetMGC()		const { return m_MGC; }
	const float GetDEF()		const { return m_DEF; }
	const float GetSPD()		const { return m_SPD; }
	const float GetBreak()		const { return m_Break; }
	const float GetBreakGauge() const { return m_BreakGauge; }

	void SetJobData(int id, ACharacterBase* character);

public:
	void LevelUp();

private:
	USkillBaseComponent* m_Skill[6];
	FString m_Name;
	int m_Level;
	int m_Exp;
	float m_HP;
	float m_MP;
	float m_ATK;
	float m_MGC;
	float m_DEF;
	float m_SPD;
	float m_Break;
	float m_BreakGauge;
};
