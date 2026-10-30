// Fill out your copyright notice in the Description page of Project Settings.

#include "MapManager.h"
#include "MapWidgetTable.h"
#include "MiniMapWidget.h"
#include "MapBoundsActor.h"
#include "MapWidget.h"
#include "Kismet/GamePlayStatics.h"

void UMapManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    m_MWT = LoadObject<UMapWidgetTable>(nullptr, TEXT("/Game/MembarFiles/Sakumoto/DataAsset/DA_MapWidgetTable.DA_MapWidgetTable"));

    //レベルのロードが完了したらOnPostLoadMapWithWorld関数を呼ぶ
    //FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMapManager::OnPostLoadMapWithWorld);

    FWorldDelegates::OnWorldInitializedActors.AddUObject(this, &UMapManager::OnWorldInitializedActors);
}

void UMapManager::OnPostLoadMapWithWorld(UWorld* world)
{
    UE_LOG(LogTemp, Warning, TEXT("Map Loaded"));

    // レベルの中に存在する AMapBoundsActor を全部探して actors に入れる
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsOfClass(world, AMapBoundsActor::StaticClass(), actors);

    EMapID mapId;

    for (auto actor : actors)
    {
        AMapBoundsActor* mapBoundsActor = Cast<AMapBoundsActor>(actor);
        mapId = mapBoundsActor->GetMapID();
    }

    TSubclassOf<UMapWidget>* widgetClass = m_MWT->m_MW.Find(mapId);
    if (widgetClass)
    {
        UMapWidget* mapWidget = CreateWidget<UMapWidget>(world, *widgetClass);
        mapWidget->InitMap(m_MWT->m_MapTexDiameterLength[mapId], m_MWT->m_MarginHalfLength[mapId]);
        mapWidget->AddToViewport();

        UE_LOG(LogTemp, Warning, TEXT("Map Created"));
    }
}

void UMapManager::OnWorldInitializedActors(const UWorld::FActorsInitializedParams& Params)
{
    UWorld* world = Params.World;

    if (!world)
    {
        return;
    }

    DebugInit(world);
}

void UMapManager::DebugInit(UWorld* world)
{
    UE_LOG(LogTemp, Warning, TEXT("Map Loaded"));

    //if (!m_MWT) return;

    // レベルの中に存在する AMapBoundsActor を全部探して actors に入れる
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsOfClass(world, AMapBoundsActor::StaticClass(), actors);

    EMapID mapId;

    for (auto actor : actors)
    {
        AMapBoundsActor* mapBoundsActor = Cast<AMapBoundsActor>(actor);
        mapId = mapBoundsActor->GetMapID();
    }

    TSubclassOf<UMapWidget>* widgetClass = m_MWT->m_MW.Find(mapId);
    TSubclassOf<UMiniMapWidget>* miniWidgetClass = m_MWT->m_MMW.Find(mapId);

    if (widgetClass)
    {
        /*UMapWidget* mapWidget = CreateWidget<UMapWidget>(world, *widgetClass);
        mapWidget->InitMap(m_MWT->m_MapTexDiameterLength[mapId], m_MWT->m_MarginHalfLength[mapId]);
        mapWidget->AddToViewport();*/
    }

    if (miniWidgetClass)
    {
        UMiniMapWidget* miniMapWidget = CreateWidget<UMiniMapWidget>(world, *miniWidgetClass);
        miniMapWidget->InitMap(m_MWT->m_MapTexDiameterLength[mapId], m_MWT->m_MarginHalfLength[mapId]);
        miniMapWidget->AddToViewport();
    }
}