// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EventScriptRow.generated.h"

USTRUCT(BlueprintType)
struct FEventScriptRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Type;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Param1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	FString Param2;

};
