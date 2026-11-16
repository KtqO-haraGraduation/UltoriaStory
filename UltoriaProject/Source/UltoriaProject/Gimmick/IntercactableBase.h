// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IntercactableBase.generated.h"

UCLASS()
class ULTORIAPROJECT_API AInteractableBase : public AActor
{
	GENERATED_BODY()
public:

	void Interact(AActor* interactor)
	{
		if (!CanInteract(interactor))return;
		OnInteract(interactor);
	}

protected:
	virtual bool CanInteract(AActor* interactor) { return true; }

	virtual void OnInteract(AActor* interactor) {}

};
