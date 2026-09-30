// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DebugMapActor.generated.h"

class UMapWidget;
class UMiniMapWidget;

UCLASS()
class ULTORIAPROJECT_API ADebugMapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebugMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
	UMapWidget* m_MapWidget;
	UPROPERTY(EditAnywhere)
	UMiniMapWidget* m_MiniMapWidget;
	UPROPERTY(EditAnywhere)
	bool m_EntireArea;
};
