#include "Buildings/Conveyor.h"

#include "Components/SplineMeshComponent.h"
#include "Graph/Nodes/ItemConveyorNode.h"

AConveyor::AConveyor()
{
    this->PrimaryActorTick.bCanEverTick = true;

    this->GraphNodeComponent = CreateDefaultSubobject<UGraphNodeComponent>(TEXT("Graph Node"));
    this->GraphNodeComponent->SetNodeClass(UItemConveyorNode::StaticClass());

    this->InItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("Input Item Connection"));
    this->InItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->InItemConnectionComponent->SetIsInput(true);

    this->OutItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("Output Item Connection"));
    this->OutItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->OutItemConnectionComponent->SetIsInput(false);

    this->ItemInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Item Instances"));
    this->ItemInstances->SetupAttachment(this->RootComponent);
    this->ItemInstances->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    this->SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline Component"));
    this->SplineComponent->SetupAttachment(this->RootComponent);
    this->SplineComponent->SetSplinePointType(0, ESplinePointType::Curve);
    this->SplineComponent->ReparamStepsPerSegment = 50;

    this->Speed                = 40.0f;
    this->DistanceBetweenItems = 50.0f;
    this->BaseSpacing          = 100.0f;
}

void AConveyor::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    ensure(this->GraphNodeComponent);

    if (UItemConveyorNode* ItemConveyorNode = Cast<UItemConveyorNode>(this->GraphNodeComponent->GetNode()))
    {
        ItemConveyorNode->SetSplineComponent(this->SplineComponent);
        ItemConveyorNode->OnItemAdded.BindUObject(this, &AConveyor::OnItemAdded);
        ItemConveyorNode->OnItemRemoved.BindUObject(this, &AConveyor::OnItemRemoved);
    }
}

void AConveyor::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    this->CreateBaseInstances();
}

void AConveyor::CompleteBuilding(UBuildingConnectionComponent* FromSnapConnection, UBuildingConnectionComponent* ToSnapConnection)
{
    Super::CompleteBuilding(FromSnapConnection, ToSnapConnection);

    FVector FirstSplinePointLocation = this->SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
    this->InItemConnectionComponent->SetRelativeLocation(FirstSplinePointLocation);

    FVector LastSplinePointLocation = this->SplineComponent->GetLocationAtSplinePoint(this->SplineComponent->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::Local);
    this->OutItemConnectionComponent->SetRelativeLocation(LastSplinePointLocation);

    if (UItemConveyorNode* ItemConveyorNode = Cast<UItemConveyorNode>(this->GraphNodeComponent->GetNode()))
    {
        ItemConveyorNode->SetStartLocation(this->GetStartLocation());

        ItemConveyorNode->Speed = this->Speed;
    }

    // Time = Distance / Speed
    this->SplineComponent->Duration = this->SplineComponent->GetSplineLength() / this->Speed;
}

void AConveyor::SetSplinePointTransform(uint32 SplinePointIndex, const FTransform &InSplineTransform)
{
    ensure(this->SplineComponent);

    FVector Tangent = FVector(300.0f, 0.0f, 0.0f);

    this->SplineComponent->SetLocationAtSplinePoint(SplinePointIndex, InSplineTransform.GetLocation(), ESplineCoordinateSpace::World);
    this->SplineComponent->SetRotationAtSplinePoint(SplinePointIndex, InSplineTransform.GetRotation().Rotator(), ESplineCoordinateSpace::World);
    this->SplineComponent->SetSplinePointType(SplinePointIndex, ESplinePointType::Curve);
    this->SplineComponent->SetTangentAtSplinePoint(SplinePointIndex, Tangent, ESplineCoordinateSpace::World);
}

void AConveyor::CreateBaseInstances()
{
    ensure(this->SplineComponent);

    this->GetComponents<USplineMeshComponent>(this->BaseSplineMeshComponents);

    for (USplineMeshComponent* SplineMesh : this->BaseSplineMeshComponents)
    {
        SplineMesh->DestroyComponent();
    }

    this->BaseSplineMeshComponents.Empty();

    int32 NumSplinePoints = this->SplineComponent->GetNumberOfSplinePoints();

    int32 NumSplineMeshComponents = NumSplinePoints - 1;

    for (int32 i = 0; i < NumSplineMeshComponents; i++)
    {
        USplineMeshComponent* BaseSplineMesh = NewObject<USplineMeshComponent>(this);
        BaseSplineMesh->SetStaticMesh(this->BaseMesh);
        BaseSplineMesh->SetMobility(EComponentMobility::Static);
        BaseSplineMesh->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
        BaseSplineMesh->SetupAttachment(this->RootComponent);

        FVector StartLocation = this->SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
        FVector StartTangent = this->SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);

        FVector EndLocation = this->SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);
        FVector EndTangent = this->SplineComponent->GetTangentAtSplinePoint(i + 1, ESplineCoordinateSpace::Local);

        BaseSplineMesh->SetForwardAxis(ESplineMeshAxis::X);
        BaseSplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true );

        BaseSplineMesh->RegisterComponent();

        this->BaseSplineMeshComponents.Add(BaseSplineMesh);
    }
}

FVector AConveyor::GetStartLocation()
{
    // Local space
    return this->SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
}

FVector AConveyor::GetEndLocation()
{
    return this->SplineComponent->GetLocationAtSplinePoint(this->SplineComponent->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::Local);
}

void AConveyor::OnItemAdded(const FConveyorItem& InItem)
{
    ensure(this->ItemInstances);

    if (InItem.Item == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Item Asset was nullptr"));
        return;
    }

    if (this->ItemInstances->GetStaticMesh() == nullptr)
    {
        this->ItemInstances->SetStaticMesh(InItem.Item->GetStaticMesh());
    }

    this->ItemInstances->AddInstance(InItem.Transform, false);
}

void AConveyor::OnItemRemoved()
{
    ensure(this->ItemInstances);

    this->ItemInstances->RemoveInstance(0);
}

void AConveyor::UpdateInstanceTransforms(const float &DeltaTime)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(AConveyor::UpdateInstanceTransforms)

    UWorld* World = GetWorld();

    ensure(World);

    this->LastTimeOnScreen = this->ItemInstances->GetLastRenderTimeOnScreen();

    // Don't update transforms if this conveyor isn't on screen
    if (World->GetTimeSeconds() - this->LastTimeOnScreen > 0.1f)
    {
        return;
    }

    UItemConveyorNode* ItemConveyorNode = Cast<UItemConveyorNode>(this->GraphNodeComponent->GetNode());
    if (!ItemConveyorNode) return;

    const TArray<FConveyorItem> &ConveyorItems = ItemConveyorNode->GetItems();

    TArray<FTransform> NewInstanceTransforms;
    {
        TRACE_CPUPROFILER_EVENT_SCOPE(AConveyor::UpdateInstanceTransforms_Internal)

        NewInstanceTransforms.Init(FTransform::Identity, ConveyorItems.Num());

        for (int IndexOfItem = 0; IndexOfItem < ConveyorItems.Num(); IndexOfItem++)
        {
            NewInstanceTransforms[IndexOfItem] = ConveyorItems[IndexOfItem].Transform;
        }
    }

    this->ItemInstances->BatchUpdateInstancesTransforms(0, NewInstanceTransforms, false, true, false);
}
