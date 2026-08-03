// Fill out your copyright notice in the Description page of Project Settings.

#include "Shop.h"
#include <EnhancedInputSubsystems.h>
#include "../Shop/ShopManager.h"
#include "../ShopMenuWidget/ShopMenuWidget.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AShop::AShop()
{
    PrimaryActorTick.bCanEverTick = false;

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    RootComponent = Mesh;

    CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
    CollisionBox->SetupAttachment(RootComponent);

    CollisionBox->SetBoxExtent(FVector(300.f));

    CollisionBox->OnComponentBeginOverlap.AddDynamic(this,&AShop::OnOverlapBegin);
        
    CollisionBox->OnComponentEndOverlap.AddDynamic(this,&AShop::OnOverlapEnd);

}

void AShop::OpenShop()
{
    // インタラクトUIを消す
    if (PromptWidget)
    {
        PromptWidget->RemoveFromParent();
    }

    // ショップUI表示
    if (ShopMenuWidgetClass)
    {
        if (!ShopMenuWidget)
        {
            ShopMenuWidget = CreateWidget<UUserWidget>(GetWorld(),ShopMenuWidgetClass);            
        }

        if (ShopMenuWidget)
        {
            ShopMenuWidget->AddToViewport();

            UShopManager* ShopManager = GetWorld()->GetSubsystem<UShopManager>();
                
            if (ShopManager)
            {
                UShopMenuWidget* Menu = Cast<UShopMenuWidget>(ShopMenuWidget);
                    
                if (Menu)
                {
                    Menu->SetupItems(ShopManager->GetCurrentItems());
                }
            }
        }
    }
}

void AShop::CloseShop()
{
    if (ShopMenuWidget)
    {
        ShopMenuWidget->RemoveFromParent();
        ShopMenuWidget = nullptr;
    }

    // まだ範囲内ならPrompt再表示
    if (PromptWidget)
    {
        PromptWidget->AddToViewport();
    }

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShop::BeginPlay()
{
	Super::BeginPlay();
	
}

UShopMenuWidget* AShop::GetShopMenuWidget() const
{
    return Cast<UShopMenuWidget>(ShopMenuWidget);
}

void AShop::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    UShopManager* shopMager = GetWorld()->GetSubsystem<UShopManager>();

    if (OtherActor)
    {
        // プレイヤーに現在のショップを通知
        shopMager->SetCurrentShop(this);

        // UI表示
        if (PromptWidgetClass)
        {
            if (!PromptWidget)
            {
                PromptWidget = CreateWidget<UUserWidget>(GetWorld(),PromptWidgetClass);   
            }

            if (PromptWidget)
            {
                PromptWidget->AddToViewport();
            }
        }
    }
}

void AShop::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    UShopManager* shopMager = GetWorld()->GetSubsystem<UShopManager>();

    if (OtherActor)
    {
        // ショップ解除
        shopMager->SetCurrentShop(nullptr);

        // UI削除
        if (PromptWidget)
        {
            PromptWidget->RemoveFromParent();
            PromptWidget = nullptr;
        }
    }
}