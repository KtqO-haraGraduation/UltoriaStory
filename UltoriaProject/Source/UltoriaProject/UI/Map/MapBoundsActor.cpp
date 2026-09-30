// Fill out your copyright notice in the Description page of Project Settings.
#include "MapBoundsActor.h"
#include "MapWidget.h"

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
	

}

// Called every frame
void AMapBoundsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

