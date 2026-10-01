// Fill out your copyright notice in the Description page of Project Settings.

#include "MapManager.h"
#include "MapWidgetTable.h"
#include "MapBoundsActor.h"
#include "MapWidget.h"
#include "Kismet/GamePlayStatics.h"


void UMapManager::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);

    m_MWT = LoadObject<UMapWidgetTable>(nullptr, TEXT("/Script/UltoriaProject.MapWidgetTable'/Game/MembarFiles/Sakumoto/DataAsset/DA_MapWidgetTable.DA_MapWidgetTable'"));

    //レベルのロードが完了したらOnPostLoadMapWithWorld関数を呼ぶ
    FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMapManager::OnPostLoadMapWithWorld);
}

void UMapManager::OnPostLoadMapWithWorld(UWorld* world)
{
    UE_LOG(LogTemp, Warning, TEXT("Map Loaded"));

    // レベルの中に存在する AMapBoundsActor を全部探して actors に入れる
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsOfClass(world, AMapBoundsActor::StaticClass(), actors);

    for (auto actor : actors)
    {
       AMapBoundsActor* mapBoundsActor = Cast<AMapBoundsActor>(actor);

        EMapID mapId = mapBoundsActor->GetMapID();

        TSubclassOf<UMapWidget>* widgetClass = m_MWT->m_MW.Find(mapId);

        if (widgetClass)
        {
            UMapWidget* mapWidget = CreateWidget<UMapWidget>(world, *widgetClass);

            mapWidget->AddToViewport();
        }
    }
}
