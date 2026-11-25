// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "JobDataRow.generated.h"

USTRUCT(BlueprintType)
struct FJobDataRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MGC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SPD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Desc;
};
