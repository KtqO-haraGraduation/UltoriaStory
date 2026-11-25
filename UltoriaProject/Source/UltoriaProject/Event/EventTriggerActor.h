// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Event/EventScriptEngine.h"
#include "EventTriggerActor.generated.h"

class UEventScriptEngine;

UCLASS()
class ULTORIAPROJECT_API AEventTriggerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventTriggerActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FName GetTriggerID() const { return m_TriggerID; }

	UFUNCTION(BlueprintCallable)
	void BeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);

private:
	// イベントシーンスクリプトエンジン
	UPROPERTY(VisibleAnywhere, Category = "Event")
	UEventScriptEngine* m_EventScriptEngine;

	// イベントシーンスクリプト
	UPROPERTY(EditAnywhere, Category = "Event")
	UDataTable* m_EventScript;

	// ストーリ進行イベント用ID
	UPROPERTY(EditAnywhere, Category = "Event")
	FName m_TriggerID;
};
