// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../Battle/Combat/Components/ComponentBase.h"
#include "AIComponentBase.generated.h"

// AIÇÃÉ^ÉCÉv
UENUM()
enum class AIType
{
	FIELD,
	BATTLE,
};

// èÛë‘
enum class AIStatus
{
	Patrol,

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API UAIComponentBase : public UComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAIComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
protected:
	AIType m_AIType;
};
