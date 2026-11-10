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

    // 対応するマップウィジェットの情報が入ったテーブルを取得
    m_MWT = LoadObject<UMapWidgetTable>(nullptr, TEXT("/Game/MembarFiles/Sakumoto/DataAsset/DA_MapWidgetTable.DA_MapWidgetTable"));
    // 
    m_IsMiniMapVisible = true;
}

void UMapManager::CreateMap(UWorld* world)
{
    if (!m_MWT)return;

    // マップIDの情報を持ったアクターを探して取得
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsOfClass(world, AMapBoundsActor::StaticClass(), actors);

    // マップID
    EMapID mapID;

    // マップIDを取得
    for (auto actor : actors)
    {
        AMapBoundsActor* mapBoundActor = Cast<AMapBoundsActor>(actor);
        mapID = mapBoundActor->GetMapID();
    }

    // マップクラスを取得
    TSubclassOf<UMapWidget>* mapWidgetClass = m_MWT->m_MW.Find(mapID);

    if (mapWidgetClass && *mapWidgetClass)
    {
        // 古いものは消す
        if (m_Map)
        {
            m_Map->RemoveFromParent();
            m_Map = nullptr;
        }

        // マップを作成
        m_Map = CreateWidget<UMapWidget>(world, *mapWidgetClass);
        // マップが作成されていれば
        if (m_Map)
        {
            // マップの初期設定をしてViewPointに追加
            m_Map->InitMap(m_MWT->m_MapTexDiameterLength[mapID], m_MWT->m_MarginHalfLength[mapID]);
            m_Map->AddToViewport();

            // 作り直されても、保存した表示状態を反映
            SetMapVisible(m_IsMapVisible);
        }

    }
}

void UMapManager::CreateMiniMap(UWorld* world)
{
    if (!m_MWT)return;

    // マップIDの情報を持ったアクターを探して取得
    TArray<AActor*> actors;
    UGameplayStatics::GetAllActorsOfClass(world, AMapBoundsActor::StaticClass(), actors);

    // マップID
    EMapID mapID;

    // マップIDを取得
    for (auto actor : actors)
    {
        AMapBoundsActor* mapBoundsActor = Cast<AMapBoundsActor>(actor);
        mapID = mapBoundsActor->GetMapID();
    }

    // ミニマップクラスを取得
    TSubclassOf<UMiniMapWidget>* miniMapWidgetClass = m_MWT->m_MMW.Find(mapID);

    if (miniMapWidgetClass && *miniMapWidgetClass)
    {
        // 古いものは消す
        if (m_MiniMap)
        {
            m_MiniMap->RemoveFromParent();
            m_MiniMap = nullptr;
        }

        // ミニマップを作成
        m_MiniMap = CreateWidget<UMiniMapWidget>(world, *miniMapWidgetClass);
        // ミニマップが作成されていれば
        if (m_MiniMap)
        {
            // マップの初期設定をしてViewPointに追加
            m_MiniMap->InitMap(m_MWT->m_MapTexDiameterLength[mapID], m_MWT->m_MarginHalfLength[mapID]);
            m_MiniMap->AddToViewport();

            // 作り直されても、保存した表示状態を反映
            //SetMiniMapVisible(m_IsMiniMapVisible);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("[Map] MiniMap created inViewport=%d"), m_MiniMap->IsInViewport());
}

void UMapManager::SetMapVisible(bool isVisible)
{
    // マップの表示フラグを適用
    m_IsMapVisible = isVisible;

    // マップウィジェットの表示/非表示
    if (m_Map)
    {
        m_Map->SetVisibility(
            isVisible ? ESlateVisibility::SelfHitTestInvisible
                      : ESlateVisibility::Collapsed);
    }
}

void UMapManager::SetMiniMapVisible(bool isVisible)
{
    // ミニマップの表示フラグを適用
    m_IsMiniMapVisible = isVisible;

    // ミニマップウィジェットの表示/非表示
    if (m_MiniMap)
    {
        m_MiniMap->SetVisibility(
            isVisible ? ESlateVisibility::SelfHitTestInvisible
                      : ESlateVisibility::Collapsed);
    }
}
