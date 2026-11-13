// Fill out your copyright notice in the Description page of Project Settings.
#include "MapBoundsActor.h"
#include "MapManager.h"

// Sets default values
AMapBoundsActor::AMapBoundsActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapBoundsActor::BeginPlay()
{
	Super::BeginPlay(); 

	UMapManager* mapManager =GetGameInstance()->GetSubsystem<UMapManager>();

	if (mapManager)
	{
		mapManager->CreateMap(GetWorld());
		mapManager->SetMapVisible(false);
		mapManager->CreateMiniMap(GetWorld());
		mapManager->SetMiniMapVisible(true);
	}
}

// Called every frame
void AMapBoundsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}