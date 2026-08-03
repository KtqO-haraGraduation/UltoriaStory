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
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MGC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SPD;
};