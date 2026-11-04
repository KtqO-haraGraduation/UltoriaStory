// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "BattleCameraManager.generated.h"

class ACameraActor;
class UBattleManager;
enum class EBattleCameraType : uint8;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleCameraManager : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(UBattleManager* battleMg);
	void SetActionCameraPos();

private:
	UBattleManager* m_BattleManager;
};
