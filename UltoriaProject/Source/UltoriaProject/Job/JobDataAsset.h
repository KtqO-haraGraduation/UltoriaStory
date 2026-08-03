// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "JobDataAsset.generated.h"

class UDataTable;
/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UJobDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> m_JobDataMappingTable;	// データ

	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> m_JobLvMappingTable;		// レベル
};
