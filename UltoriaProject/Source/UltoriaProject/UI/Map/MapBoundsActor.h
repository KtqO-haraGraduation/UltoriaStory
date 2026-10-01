// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapWidgetTable.h"
#include "MapBoundsActor.generated.h"

UCLASS()
class ULTORIAPROJECT_API AMapBoundsActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapBoundsActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	EMapID GetMapID() { return m_MapID; }

private:
	UPROPERTY(EditAnywhere, Category = "Map")
	EMapID m_MapID;
};
