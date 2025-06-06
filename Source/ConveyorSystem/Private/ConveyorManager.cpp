#include "ConveyorManager.h"

#include "Graph/Nodes/ItemConveyorNode.h"
#include "Kismet/GameplayStatics.h"

TWeakObjectPtr<AConveyorManager> AConveyorManager::Instance = nullptr;

AConveyorManager::AConveyorManager()
{
    this->PrimaryActorTick.bCanEverTick = true;

    this->ItemTransportGraph = CreateDefaultSubobject<UItemTransportGraph>(TEXT("Item Transport Graph"));

    if (!AConveyorManager::Instance.IsValid())
    {
        AConveyorManager::Instance = this;
    }

}

void AConveyorManager::BeginPlay()
{
    Super::BeginPlay();

    this->PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    TArray<AActor*> OutConveyors;

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AConveyor::StaticClass(), OutConveyors);

    for (AActor* ConveyorActor : OutConveyors)
    {
        this->Conveyors.Add(Cast<AConveyor>(ConveyorActor));
    }
}

void AConveyorManager::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    TArray<UItemTransportNode*> TopologicalOrder = this->ItemTransportGraph->GetTopologicalOrder();

    // All items on every conveyor need to moved before pushing/pulling from neighboring nodes every tick
    this->UpdateAllItemTransports(TopologicalOrder, DeltaSeconds);
    this->ProcessAllItemTransports(TopologicalOrder, DeltaSeconds);

    this->UpdateAllConveyors(DeltaSeconds);
}

void AConveyorManager::UpdateAllItemTransports(const TArray<UItemTransportNode*>& Order, float DeltaSeconds)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_UpdateAllItemTransports);
    TRACE_CPUPROFILER_EVENT_SCOPE(AConveyorManager::UpdateAllItemTransports)

    ParallelFor(Order.Num(), [&Order, DeltaSeconds](uint32 Index)
    {
        Order[Index]->UpdateItems(DeltaSeconds);
    });
}

void AConveyorManager::ProcessAllItemTransports(const TArray<UItemTransportNode*>& Order, float DeltaSeconds)
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_ProcessAllItemTransports);
    TRACE_CPUPROFILER_EVENT_SCOPE(AConveyorManager::ProcessAllItemTransports)

    for (int i = 0; i < Order.Num(); i++)
    {
        UItemTransportNode* CurrentNode = Order[i];

        if (!CurrentNode) continue;

        CurrentNode->ProcessItems(DeltaSeconds);
    }
}

void AConveyorManager::UpdateAllConveyors(float DeltaSeconds)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(AConveyorManager::UpdateAllConveyors)

    const TArray<AConveyor*>& TempConveyors = this->Conveyors;

    for (int i = 0; i < TempConveyors.Num(); i++)
    {
        AConveyor* CurrentConveyor = TempConveyors[i];

        if (CurrentConveyor)
        {
            CurrentConveyor->UpdateInstanceTransforms(DeltaSeconds);
        }
    }
}

UItemTransportGraph* AConveyorManager::GetGraph()
{
    return this->ItemTransportGraph;
}

void AConveyorManager::AddItemToConveyor(UItemDataAsset* InItem)
{
    if (!this->ItemTransportGraph) return;

    UNodeBase* Node = this->ItemTransportGraph->GetAllSourceNodes().Last();

    UItemConveyorNode* ItemConveyorNode = Cast<UItemConveyorNode>(Node);

    if (ItemConveyorNode && ItemConveyorNode->CanAddItem())
    {
        ItemConveyorNode->AddItem(InItem);
    }
}

void AConveyorManager::AddItemToAllSourceConveyors(UItemDataAsset* InItem)
{
    if (!this->ItemTransportGraph) return;

    for (UNodeBase* Node : this->ItemTransportGraph->GetAllSourceNodes())
    {
        UItemConveyorNode* ItemConveyorNode = Cast<UItemConveyorNode>(Node);

        if (ItemConveyorNode && ItemConveyorNode->CanAddItem())
        {
            ItemConveyorNode->AddItem(InItem);
        }
    }
}

AConveyorManager* AConveyorManager::Get()
{
    if (AConveyorManager::Instance.IsValid())
    {
        return AConveyorManager::Instance.Get();
    }

    UE_LOG(LogTemp, Error, TEXT("A ConveyorManager actor does not exist in the level, please add one."));

    return nullptr;
}
