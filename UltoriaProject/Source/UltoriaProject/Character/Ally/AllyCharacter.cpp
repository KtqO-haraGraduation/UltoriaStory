// Fill out your copyright notice in the Description page of Project Settings.

#include "AllyCharacter.h"

// Sets default values
AAllyCharacter::AAllyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAllyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AAllyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAllyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
