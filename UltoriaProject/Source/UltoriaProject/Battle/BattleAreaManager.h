// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BattleAreaManager.generated.h"

struct FBattleTransform
{
	FVector pos;
	FRotator rot;
};

UENUM()
enum class EBattleCameraType : uint8
{
	MAIN   = 0,	// バトル全体を移すカメラ
	ACTION = 1,	// 行動キャラクターを移すカメラ
};

UCLASS()
class ULTORIAPROJECT_API ABattleAreaManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABattleAreaManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// バトル時の配置座標を返す
	// count : パーティーの人数, isEnemy : 渡すのが敵かどうか
	TArray<FBattleTransform> GetBattleTransform(int count, bool isEnemy);

	// バトルカメラを取得
	ACameraActor* GetCamera(EBattleCameraType type) const;
	TArray<ACameraActor*> GetBattleCameras() const { return m_BattleCameras; }

	// バトルエリアの中心座標
	FVector GetBattleCenterPos() const { return m_BattleCenterPos; }

private:
	UPROPERTY(EditAnywhere)
	TArray<ACameraActor*> m_BattleCameras;

	FVector m_BattleCenterPos;

	// 中心点から座標をどれだけずらすか
	UPROPERTY(EditAnywhere)
	FVector2D m_Offset;

	UPROPERTY(EditAnywhere)
	USoundBase* m_BGM;
};
