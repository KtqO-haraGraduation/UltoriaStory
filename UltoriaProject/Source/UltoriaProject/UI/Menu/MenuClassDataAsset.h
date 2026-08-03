// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MenuClassDataAsset.generated.h"

class UMenuBase;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UMenuClassDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Menu")
	TSubclassOf<UMenuBase> MainMenuClass;

	UPROPERTY(EditAnywhere, Category = "Menu")
	TSubclassOf<UMenuBase> StatesMenuClass;

	UPROPERTY(EditAnywhere, Category = "Menu")
	TSubclassOf<UMenuBase> ItemsMenuClass;

	UPROPERTY(EditAnywhere, Category = "Menu")
	TSubclassOf<UMenuBase> QuestMenuClass;

	UPROPERTY(EditAnywhere, Category = "Menu")
	TSubclassOf<UMenuBase> OptionMenuClass;

	UPROPERTY(EditAnywhere, Category = "Menu")
	TSubclassOf<UMenuBase> SaveMenuClass;

	UPROPERTY(EditAnywhere, Category = "Menu")
	TSubclassOf<UMenuBase> EndMenuClass;
};
