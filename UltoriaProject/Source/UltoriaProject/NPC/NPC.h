// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/SphereComponent.h"
#include "COmponents/WidgetComponent.h"
#include "../Event/EventScriptEngine.h"
#include "NPC.generated.h"

UCLASS()
class ULTORIAPROJECT_API ANPC : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* playerInputComponent) override;

	// 接近検知関数
	UFUNCTION(BlueprintCallable)
	virtual void ApprochBeginOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool bFromSweep, const FHitResult& sweepResult);
	// 離脱検知関数
	UFUNCTION(BlueprintCallable)
	virtual void ApprochEndOverlap(UPrimitiveComponent* overlappedComp, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex);

	// 会話イベント開始関数
	void Speak();

private:
	// インタラクトUI表示/非表示関数
	void ShowIntaractUI();
	void HideIntaractUI();

public:
	// イベントシーンスクリプトエンジン
	UPROPERTY(VisibleAnywhere, Category = "Event")
	UEventScriptEngine* m_EventScriptEngine;
	// イベントシーンスクリプト
	UPROPERTY(EditAnywhere, Category = "Event")
	UDataTable* m_EventScript;

	// インタラクトUI用コンポーネント
	UPROPERTY(VisibleAnywhere, Category = "Dialogue")
	UWidgetComponent* m_InteractWidgetComponent;

private:
	// 会話可能フラグ
	bool m_CanSpeak;
};
