// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IntercactableBase.h"
#include "Door.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API ADoor : public AInteractableBase
{
	GENERATED_BODY()
	
protected:
	virtual void OnInteract(AActor* interactor) override;

	// äJï¬ÉAÉjÉÅ
	UFUNCTION(BlueprintImplementableEvent)
	void PlayOpenAnimation(bool bOpen);

private:
	bool m_IsOpen = false;
};
