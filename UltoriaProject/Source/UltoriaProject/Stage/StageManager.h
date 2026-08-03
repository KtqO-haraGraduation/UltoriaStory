// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "StageManager.generated.h"

UENUM()
enum class EStageType : uint8
{
	FOREST,			// 森（ステージ１）
	CAVE,			// 洞窟（ステージ２）
	POISON_SWAMP,	// 毒の沼（ステージ３）
	CASTLE,			// 城（ステージ４）
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UStageManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	// Get関数
	EStageType GetStageType() const { return m_StageType; }

	// Set関数
	void SetStageType(EStageType type) { if(m_StageType != type) m_StageType = type; }

private:
	EStageType m_StageType = EStageType::FOREST;
};
