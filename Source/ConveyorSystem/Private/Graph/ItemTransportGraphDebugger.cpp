// Copyright Joshua Gangl. All Rights Reserved.

#include "Graph/ItemTransportGraphDebugger.h"

#include "Kismet/GameplayStatics.h"
#include "Buildings/Conveyor.h"
#include "ConveyorManager.h"
#include "Graph/Nodes/ItemConveyorNode.h"

AItemTransportGraphDebugger::AItemTransportGraphDebugger()
{
    this->PrimaryActorTick.bCanEverTick = true;

    this->Graph    = nullptr;
    this->bEnabled = true;

    this->DistanceBetweenSplineDebugSpheres = 100.0f;

    this->bDrawUpdateOrder     = true;
    this->bDrawItemLocations   = true;
    this->bDrawSplineLocations = true;
}

void AItemTransportGraphDebugger::BeginPlay()
{
    Super::BeginPlay();

    AConveyorManager* ConveyorManager = AConveyorManager::Get();

    if (!ConveyorManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("Couldn't find a conveyor manager"));
        return;
    }

    UGraphBase* FoundGraph = ConveyorManager->GetGraph();

    this->SetGraph(FoundGraph);
}

void AItemTransportGraphDebugger::Tick(float DeltaSeconds)
{
    if (this->bEnabled)
    {
        this->DrawGraph();

        if (this->bDrawUpdateOrder)
        {
            this->DrawUpdateOrder();
        }

        if (this->bDrawItemLocations)
        {
            this->DrawItemLocations();
        }

        if (this->bDrawSplineLocations)
        {
            this->DrawSplineLocations();
        }
    }
}

void AItemTransportGraphDebugger::DrawGraph()
{
    if (!this->Graph)
    {
        UE_LOG(LogTemp, Error, TEXT("%hs : Graph is nullptr"), __FUNCTION__);
        return;
    }

    FVector Offset = FVector(0.0f, 0.0f, 100.0f);

    // Draw Node locations
    for (UNodeBase* Node : this->Graph->GetNodes())
    {
        if (!Node) continue;

        FVector Center = Node->GetLocation() + Offset;

        DrawDebugSolidBox(GetWorld(), Center, FVector(20.0f, 20.0f, 20.0f), FColor::Blue, false, -1, SDPG_MAX);
    }

    for (UEdgeBase* Edge : this->Graph->GetEdges())
    {
        if (!Edge || !Edge->Source || !Edge->Destination)
        {
            UE_LOG(LogTemp, Warning, TEXT("Edge is null"));
            continue;
        }

        FVector Direction = (Edge->Source->GetLocation() - Edge->Destination->GetLocation());
        Direction.Normalize();

        DrawDebugDirectionalArrow(GetWorld(),
                                  Edge->Source->GetLocation() - (Direction*20.0f) + Offset,
                                  Edge->Destination->GetLocation() + (Direction*20.0f) + Offset,
                                  200.0f,
                                  FColor::Yellow,
                                  false,
                                  -1,
                                  SDPG_Foreground,
                                  3);
    }

/*
    // Draw Edges between Nodes
    for (FEdgePair UniqueEdge: UniqueEdges)
    {
        FVector Direction = (UniqueEdge.Key->GetLocation() - UniqueEdge.Value->GetLocation());
        Direction.Normalize();

        DrawDebugLine(GetWorld(), UniqueEdge.Key->GetLocation() - (Direction*20.0f), UniqueEdge.Value->GetLocation() + (Direction*20.0f), FColor::Yellow, false, -1, SDPG_Foreground, 3);
    }
*/
}


void AItemTransportGraphDebugger::DrawUpdateOrder()
{
    if (!this->Graph)
    {
        return;
    }

    // Not sure why this is necessary, but without it the debug strings aren't drawn correctly
    FlushDebugStrings(GetWorld());

    UItemTransportGraph* ItemTransportGraph = Cast<UItemTransportGraph>(this->Graph);

    if (!ItemTransportGraph) return;

    TArray<UItemTransportNode*> TopOrder = ItemTransportGraph->GetTopologicalOrder();

    for (int i = 0; i < TopOrder.Num(); i++)
    {
        UItemTransportNode* CurrentNode = TopOrder[i];

        if (!CurrentNode) continue;

        FVector Loc = CurrentNode->GetLocation() + FVector(0.0f, 0.0f, 150.0f);

        // Draw a number;
        DrawDebugString(GetWorld(), Loc, FString::FromInt(i), this);
    }
}

void AItemTransportGraphDebugger::DrawItemLocations()
{
    UItemTransportGraph* ItemTransportGraph = Cast<UItemTransportGraph>(this->Graph);

    if (!ItemTransportGraph) return;

    TArray<FVector> ItemLocations;

    for (UNodeBase* Node : ItemTransportGraph->GetNodes())
    {
        UItemConveyorNode* ItemConveyorNode = Cast<UItemConveyorNode>(Node);

        if (ItemConveyorNode)
        {
            ItemConveyorNode->GetItemLocations(ItemLocations);
        }
    }

    for (const FVector& ItemLoc : ItemLocations)
    {
        DrawDebugSphere(GetWorld(),
                        ItemLoc,
                        20.0f,
                        10,
                        FColor::White);
    }
}

void AItemTransportGraphDebugger::DrawSplineLocations()
{
    TArray<AActor*> OutActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AConveyor::StaticClass(), OutActors);

    for (AActor* Actor : OutActors)
    {
        AConveyor* Conveyor = Cast<AConveyor>(Actor);

        USplineComponent* SplineComponent = Cast<USplineComponent>(Conveyor->GetComponentByClass(USplineComponent::StaticClass()));

        if (SplineComponent)
        {
            int32 NumPoints = SplineComponent->GetSplineLength() / this->DistanceBetweenSplineDebugSpheres;

            for (int i = 0; i < NumPoints; i++)
            {
                float CurrentDistance = i * this->DistanceBetweenSplineDebugSpheres;
                FVector Location = SplineComponent->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
                DrawDebugSphere(GetWorld(),
                                Location,
                                20.0f,
                                10,
                                FColor::White);
            }
        }
    }
}

