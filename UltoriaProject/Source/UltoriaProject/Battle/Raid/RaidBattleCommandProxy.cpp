#include "RaidBattleCommandProxy.h"
#include "RaidBattleManager.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"

ARaidBattleCommandProxy::ARaidBattleCommandProxy()
{
    bReplicates = true;
    SetReplicateMovement(false);
}

void ARaidBattleCommandProxy::Initialize(ARaidBattleManager* Manager, APlayerController* OwningController)
{
    if (!HasAuthority()) return;

    m_RaidManager = Manager;
    SetOwner(OwningController);
    ForceNetUpdate();
}

void ARaidBattleCommandProxy::SubmitCommand(ERaidCommandType Command, ACharacterBase* Target)
{
    ServerSubmitCommand(Command, Target);
}

void ARaidBattleCommandProxy::ServerSubmitCommand_Implementation(ERaidCommandType Command, ACharacterBase* Target)
{
    if (!m_RaidManager) return;

    m_RaidManager->HandlePlayerCommand(Cast<APlayerController>(GetOwner()), Command, Target);
}

void ARaidBattleCommandProxy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(ARaidBattleCommandProxy, m_RaidManager);
}
