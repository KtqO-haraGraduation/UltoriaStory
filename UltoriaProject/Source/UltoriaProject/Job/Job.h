// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Job.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UJob : public UObject
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UJob();

public:
	const float GetHP()			const { return m_HP; }
	const float GetMP()			const { return m_MP; }
	const float GetATK()		const { return m_ATK; }
	const float GetMGC()		const { return m_MGC; }
	const float GetDEF()		const { return m_DEF; }
	const float GetSPD()		const { return m_SPD; }
	const float GetBreak()		const { return m_Break; }
	const float GetBreakGauge() const { return m_BreakGauge; }

private:
	float m_HP;			// ‘Ì—Í
	float m_MP;			// 
	float m_ATK;		// 
	float m_MGC;		// 
	float m_DEF;		// 
	float m_SPD;		// 
	float m_Break;		// 
	float m_BreakGauge;	// 
};
