// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "../Event/EventScriptEngine.h"
#include "Engine/DataTable.h"
#include "DialogueTriggerActor.generated.h"

UCLASS()
class ULTORIAPROJECT_API ADialogueTriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADialogueTriggerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
	UBoxComponent* m_TriggerBox; 

	UPROPERTY(VisibleAnywhere, Category = "Event")
	UEventScriptEngine* m_EventScriptEngine;

	UPROPERTY(EditAnywhere, Category = "Event")
	UDataTable* m_EventScript;
	

private:
	// トリガーボックスのオーバーラップイベント関数
	UFUNCTION(BlueprintCallable)
	void TriggerBeginOverlap(UPrimitiveComponent* overlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComponent, int32 otherBodyIndex, bool bFromSpeed, const FHitResult& SweepResult);

};
