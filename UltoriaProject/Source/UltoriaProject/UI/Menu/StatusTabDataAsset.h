// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StatusTabDataAsset.generated.h"

class UMenuBase;

UCLASS()
class ULTORIAPROJECT_API UStatusTabDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Tab")
	TSubclassOf<UMenuBase> m_StrengthTabClass;

	UPROPERTY(EditAnywhere, Category = "Tab")
	TSubclassOf<UMenuBase> m_EquipTabClass;

	UPROPERTY(EditAnywhere, Category = "Tab")
	TSubclassOf<UMenuBase> m_JobTabClass;
};
