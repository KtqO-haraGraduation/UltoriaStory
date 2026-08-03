// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugMapActor.h"
#include "MapWidget.h"
#include "Blueprint/UserWidget.h"

// Sets default values
ADebugMapActor::ADebugMapActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADebugMapActor::BeginPlay()
{
	Super::BeginPlay();

	// BPで設定したウィジェットを表示
	if (m_MapWidget)
	{
		m_MapWidget->AddToViewport();
	}
}

// Called every frame
void ADebugMapActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

