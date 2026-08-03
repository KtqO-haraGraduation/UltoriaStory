// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "../NPC/NPC.h"
#include "../Event/EventScriptEngine.h"
#include "../Data/EquipItemRow.h"
#include "TestPlayer.generated.h"

UCLASS(Blueprintable)
class ULTORIAPROJECT_API ATestPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATestPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InputMove(const FInputActionValue& Value);
	void TrySpeakNPC();
	void NextSpeak();
	void OpenMenu();
	void CloseMenu();
	void MenuNavigate(const FInputActionValue& Value);
	void MenuConfirm();

	void SetNearNPC(ANPC* NPC) { m_NearNPC = NPC; }
	void SetCurrentScriptEngine(UEventScriptEngine* engine) { m_CurrentScriptEngine = engine; }

	UFUNCTION(BlueprintCallable)
	void EquipItem(EEquipSlot Slot, FName id);

	UFUNCTION(BlueprintCallable)
	FName GetEquippedId(EEquipSlot Slot);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputMappingContext* m_InputMap;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_InputMove;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_InputIntaract;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_InputTalkEnter;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_InputOpenMenu;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_InputCloseMenu;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_InputMenuNavigate;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* m_InputMenuConfirm;

	UPROPERTY(EditAnywhere, Category = "Input")
	float m_speed;

	ANPC* m_NearNPC;

	UPROPERTY()
	UEventScriptEngine* m_CurrentScriptEngine;

	UPROPERTY()
	TMap<EEquipSlot, FName> m_EquippedIds;
};
