#include "Buildings/CSBuildable.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

#include "ConveyorManager.h"

ACSBuildable::ACSBuildable()
{
    this->PrimaryActorTick.bCanEverTick = false;
}

void ACSBuildable::BeginPlay()
{
    Super::BeginPlay();

    this->CompleteBuilding(nullptr, nullptr);

    this->UpdateGraphConnections();
}

void ACSBuildable::CompleteBuilding(UBuildingConnectionComponent* FromSnapConnection, UBuildingConnectionComponent* ToSnapConnection)
{
    Super::CompleteBuilding(FromSnapConnection, ToSnapConnection);

    UGraphNodeComponent* GraphNodeComponent = Cast<UGraphNodeComponent>(this->GetComponentByClass(UGraphNodeComponent::StaticClass()));
    if (!GraphNodeComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("No graph node component"));
        return;
    }

    GraphNodeComponent->SetNodeLocation(this->GetActorLocation());

    AConveyorManager* ConveyorManager = AConveyorManager::Get();

    if (!ConveyorManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Couldn't find a conveyor manager"));
        return;
    }

    UGraphBase* Graph = ConveyorManager->GetGraph();

    if (!Graph)
    {
        UE_LOG(LogTemp, Warning, TEXT("Couldn't find a graph on conveyor manager"));
        return;
    }

    Graph->AddNode(GraphNodeComponent->GetNode());
}

void ACSBuildable::AddConnection(UBuildingConnectionComponent* FromConnection,
                                 UBuildingConnectionComponent* ToConnection)
{
    Super::AddConnection(FromConnection, ToConnection);

    UGraphNodeComponent* GraphNodeComponent = Cast<UGraphNodeComponent>(this->GetComponentByClass(UGraphNodeComponent::StaticClass()));
    if (!GraphNodeComponent)
    {
        return;
    }

    AConveyorManager* ConveyorManager = AConveyorManager::Get();

    if (!ConveyorManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Couldn't find a conveyor manager"));
        return;
    }

    UGraphBase* Graph = ConveyorManager->GetGraph();

    if (!Graph)
    {
        UE_LOG(LogTemp, Warning, TEXT("Couldn't find a graph"));
        return;
    }

    if (!FromConnection || !ToConnection)
    {
        return;
    }

    AActor* SnapToOwner = ToConnection->GetOwner();
    if (!SnapToOwner)
    {
        UE_LOG(LogTemp, Error, TEXT("%hs : OtherOwner is nullptr"), __FUNCTION__);
        return;
    }

    UGraphNodeComponent* SnapToNode = SnapToOwner->GetComponentByClass<UGraphNodeComponent>();

    if (!SnapToNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("Couldn't find a graph node component on the snapped connection"));
        return;
    }

    FromConnection->SetConnectedComponent(ToConnection);
    ToConnection->SetConnectedComponent(FromConnection);

    UNodeBase* InputNode = nullptr;
    UNodeBase* OutputNode = nullptr;

    if (ToConnection->IsInput())
    {
        InputNode = GraphNodeComponent->GetNode();
        OutputNode = SnapToNode->GetNode();
    }
    else
    {
        InputNode = SnapToNode->GetNode();
        OutputNode = GraphNodeComponent->GetNode();
    }

    Graph->AddEdge(InputNode, OutputNode);
}

void ACSBuildable::UpdateGraphConnections()
{
    // Try to connect nodes by distance
    TArray<TEnumAsByte<EObjectTypeQuery> > ObjectTypes { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic) };
    TArray<AActor*> ActorsToIgnore{this};

    for (UBuildingConnectionComponent* BuildingConnectionComponent : this->BuildingConnectionComponents)
    {
        if (!BuildingConnectionComponent) continue;

        // Ignore already connected connections
        if (BuildingConnectionComponent->IsConnected())
        {
            continue;
        }

        TArray<UPrimitiveComponent*> OutComponents;

        UKismetSystemLibrary::SphereOverlapComponents(GetWorld(),
                                                      BuildingConnectionComponent->GetSnapTransform().GetLocation(),
                                                      1.0f,
                                                      ObjectTypes,
                                                      USphereComponent::StaticClass(),
                                                      ActorsToIgnore,
                                                      OutComponents);

        // Should expect only 1 hit
        if (OutComponents.IsEmpty())
        {
            continue;
        }

        UBuildingConnectionComponent* OtherConnectionComponent = Cast<UBuildingConnectionComponent>(OutComponents[0]->GetAttachParent());

        if (!OtherConnectionComponent) continue;

        if (OtherConnectionComponent->IsConnected())
        {
            UE_LOG(LogTemp, Warning, TEXT("Other connection is already connected"));
            continue;
        }

        this->AddConnection(BuildingConnectionComponent, OtherConnectionComponent);
    }
}
