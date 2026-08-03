// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Shop.generated.h"


class UBoxComponent;
class UStaticMeshComponent;
class UUserWidget;

UCLASS()
class ULTORIAPROJECT_API AShop : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AShop();

    void OpenShop();
    void CloseShop();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
    class UShopMenuWidget* GetShopMenuWidget() const;
    FName GetShopCategory() const { return ShopCategory; }

public:	
    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* Mesh;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* CollisionBox;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> PromptWidgetClass;

    UPROPERTY()
    UUserWidget* PromptWidget;

    UPROPERTY(EditAnywhere, Category = "UI")
    TSubclassOf<UUserWidget> ShopMenuWidgetClass;

    UPROPERTY()
    UUserWidget* ShopMenuWidget;

    // ショップカテゴリー
    UPROPERTY(EditAnywhere)
    FName ShopCategory;


    UFUNCTION()
    void OnOverlapBegin(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult
    );

    UFUNCTION()
    void OnOverlapEnd(
        UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex
    );
};
