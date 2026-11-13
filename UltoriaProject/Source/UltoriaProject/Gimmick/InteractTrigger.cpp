// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractTrigger.h"
#include "../System/Controller/PlayPlayerController.h"

// Sets default values
AInteractTrigger::AInteractTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AInteractTrigger::TryInteract(AActor* interactor)
{
	if (m_Target) m_Target->Interact(interactor);
}

void AInteractTrigger::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* Other,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* pawn = Cast<APawn>(Other);
	if (!pawn) return;

	APlayPlayerController* pc =
		Cast<APlayPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc) pc->SetCurrentGimmickTrigger(this);
}

void AInteractTrigger::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* Other,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APawn* pawn = Cast<APawn>(Other);
	if (!pawn) return;

	APlayPlayerController* pc =
		Cast<APlayPlayerController>(GetWorld()->GetFirstPlayerController());

	if (pc) pc->ClearCurrentGimmickTrigger(this);
}

