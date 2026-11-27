// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "JobDataRow.h"
#include "../Battle/Combat/Components/ComponentBase.h"
#include "JobDataComponent.generated.h"

class ACharacterBase;
class USkillBaseComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API UJobDataComponent : public UComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJobDataComponent();

public:
	// 
	void BeginPlay() override;

public:
	//USkillBaseComponent* GetSkill(int id) { return m_Skill[id]; }
	USkillBaseComponent* GetSkill() { return m_Skill; }
	const FName GetName()	const { return m_Data.Name; }
	const int	GetLevel()			const { return m_Level; }
	const int	GetExp()			const { return m_Exp; }
	const float GetHP()			const { return m_Data.HP; }
	const float GetMP()			const { return m_Data.MP; }
	const float GetATK()		const { return m_Data.ATK; }
	const float GetMGC()		const { return m_Data.MGC; }
	const float GetDEF()		const { return m_Data.DEF; }
	const float GetSPD()		const { return m_Data.SPD; }
	const float GetBreak()		const { return m_Break; }
	const float GetBreakGauge() const { return m_BreakGauge; }

	void SetJobData(int id, ACharacterBase* character);

	void SetJobData(FName jobName);

public:
	bool CanLevelUp();
	void LevelUp();

	void CreateSkill();

private:
	USkillBaseComponent* m_Skill;
	FJobDataRow m_Data;
	int m_Level;
	int m_Exp;

	float m_Break;
	float m_BreakGauge;
};
