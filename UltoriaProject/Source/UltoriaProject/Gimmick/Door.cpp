// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

void ADoor::OnInteract(AActor* interactor)
{
	m_IsOpen = !m_IsOpen;
	PlayOpenAnimation(m_IsOpen);
}
