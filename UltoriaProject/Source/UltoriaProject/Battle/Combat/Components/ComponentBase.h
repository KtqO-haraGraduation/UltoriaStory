// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ComponentBase.generated.h"

class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API UComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UComponentBase();

	virtual void Update() {}

	bool IsActiveComponent() const { return m_IsActive; }
	void SetActiveComponent(bool active) { if(m_IsActive != active) m_IsActive = active; }

	ACharacterBase* GetCharacterOwner() const { return m_Owner; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	bool m_IsActive = true;
	ACharacterBase* m_Owner;	// コンポーネントがついてるオーナーを保持
};
