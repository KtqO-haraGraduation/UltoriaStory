// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "EnemyBase.generated.h"

class UWidgetComponent;
class ABattleAreaManager;

UCLASS()
class ULTORIAPROJECT_API AEnemyBase : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// パーティーを生成
	TArray<ACharacterBase*> CreateEnemyParty();

	// 最終ステータス
	FStatus GetTotalStatus() const override;

	// Set関数
	// UIの表示、非表示を切り替える
	void SetStatusUIVisibility(bool flag);
	void SetSelectionUIVisibility(bool flag);

	void SetStop(bool bStop) { m_IsStop = bStop; }

	void SetBaseStatus() override;

protected:
	bool m_IsStop; // 止めるか

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* m_StatusUIWidget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* m_SelectionUIWidget;
};
