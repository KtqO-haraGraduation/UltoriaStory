// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyNetworkGameInstance.generated.h"

class AMyNetworkPlayerController;
/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UMyNetworkGameInstance : public UGameInstance
{
	GENERATED_BODY()

	friend class AMyNetworkPlayerController;

private:

	bool IsUICreate = false;

	bool IsInLobby = false;
};
