#include "Graph/Nodes/ItemConveyorNode.h"

UItemConveyorNode::UItemConveyorNode()
{
    this->CurrentItems.Empty();

    this->Speed                = 40.0f;
    this->DistanceBetweenItems = 50.0f;

    // TODO: This needs to be the spline length?
    this->Length               = 5.0f;

    this->SplineComponent = nullptr;

    this->StartLocation = FVector::Zero();
    this->EndLocation   = FVector::Zero();
}

void UItemConveyorNode::UpdateItems(float DeltaSeconds)
{
    Super::UpdateItems(DeltaSeconds);

    // TODO: Update item locations
    if (this->CurrentItems.IsEmpty() || !this->SplineComponent) return;

    const float DistanceToMove = this->Speed * DeltaSeconds;

    FVector ItemOffset = FVector(0.0f, 0.0f, 50.0f);

    for (int IndexOfTransform = 0; IndexOfTransform < this->CurrentItems.Num(); IndexOfTransform++)
    {
        FTransform &ItemTransform = this->CurrentItems[IndexOfTransform].Transform;

        const FVector& CurrentLocation = ItemTransform.GetLocation();

        // Check if this transform is close enough to next item
        if (this->CurrentItems.IsValidIndex(IndexOfTransform - 1))
        {
            // Get next item on conveyor
            const FTransform &NextItemTransform = this->CurrentItems[IndexOfTransform - 1].Transform;

            float DistOfCurrentItem = this->SplineComponent->GetDistanceAlongSplineAtLocation(CurrentLocation, ESplineCoordinateSpace::Local);
            float DistOfNextItem = this->SplineComponent->GetDistanceAlongSplineAtLocation(NextItemTransform.GetLocation(), ESplineCoordinateSpace::Local);

            const float DistanceToNextItem = DistOfNextItem - DistOfCurrentItem;

            // There is enough room to move this item
            if (DistanceToNextItem > this->DistanceBetweenItems)
            {
                float ClampedDistance = FMath::Min(DistanceToMove, this->DistanceBetweenItems);
                FVector NewLocation = this->SplineComponent->GetLocationAtDistanceAlongSpline(DistOfCurrentItem + ClampedDistance, ESplineCoordinateSpace::Local);
                NewLocation += ItemOffset;
                ItemTransform.SetLocation(NewLocation);

                continue;
            }
            else
            {
                continue;
            }
        }

        float Dist = this->SplineComponent->GetDistanceAlongSplineAtLocation(CurrentLocation, ESplineCoordinateSpace::Local);

        float NewDistance = Dist + DistanceToMove;

        FVector NewLocation = this->SplineComponent->GetLocationAtDistanceAlongSpline(NewDistance, ESplineCoordinateSpace::Local);

        NewLocation += ItemOffset;

        ItemTransform.SetLocation(NewLocation);
    }
}

void UItemConveyorNode::AddItem(UItemDataAsset* InItem)
{
    if (!InItem)
    {
        UE_LOG(LogTemp, Error, TEXT("%hs : InItem is nullptr"), __FUNCTION__);
        return;
    }

    FTransform ItemTransform;

    FVector ItemOffset = FVector(0.0f, 0.0f, 50.0f);

    ItemTransform.SetLocation(this->GetStartLocation() + ItemOffset);

    // Add transform to end of array
    // This array will always be sorted in reverse order with the last item transform being closest to the end
    const FConveyorItem NewConveyorItem = FConveyorItem(InItem, ItemTransform);

    this->CurrentItems.Add(NewConveyorItem);

    this->OnItemAdded.ExecuteIfBound(InItem);
}

bool UItemConveyorNode::CanAddItem()
{
    // We need to check if the last item in the item array is too close to the start

    if (this->CurrentItems.Num() == 0) return true;

    const FVector &FirstItemLocation = this->CurrentItems.Last().Transform.GetLocation();

    float DistanceToStart = this->SplineComponent->GetDistanceAlongSplineAtLocation(FirstItemLocation, ESplineCoordinateSpace::Local);

    if (DistanceToStart < this->DistanceBetweenItems)
    {
        return false;
    }

    return true;
}

bool UItemConveyorNode::CanGrabItem()
{
    if (this->CurrentItems.Num() == 0)
    {
        return false;
    }

    // TODO: We should cache the state of if a conveyor can be grabbed from to help performance,
    //   which can be updated in UpdateItems()

    const FVector &CurrentLocation = this->CurrentItems[0].Transform.GetLocation();

    FVector SplineEnd = this->SplineComponent->GetLocationAtDistanceAlongSpline(this->SplineComponent->GetSplineLength(),
                                                                                ESplineCoordinateSpace::Local);

    const float DistanceToEnd = FVector::Distance(CurrentLocation, SplineEnd + FVector(0.0f, 0.0f, 50.0f));

    // TODO: Update this logic, i dont think it will be correct with splines
    if (FMath::IsNearlyEqual(DistanceToEnd, 0.0, 0.01f))
    {
        return true;
    }

    return false;
}

UItemDataAsset* UItemConveyorNode::TryRemoveItemFromEnd()
{
    if (!this->CanGrabItem())
    {
        return nullptr;
    }

    FConveyorItem& ItemToRemove = this->CurrentItems[0];

    UItemDataAsset* ItemAssetToRemove = ItemToRemove.Item;

    // Remove the first item
    // TODO: Should the CurrentItems Array be a queue instead?
    this->CurrentItems.RemoveAt(0);

    this->OnItemRemoved.ExecuteIfBound();

    return ItemAssetToRemove;
}

FVector UItemConveyorNode::GetStartLocation()
{
    ensure(this->SplineComponent);

    return this->SplineComponent->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
}

FVector UItemConveyorNode::GetEndLocation()
{
    ensure(this->SplineComponent);

    return this->SplineComponent->GetLocationAtSplinePoint(this->SplineComponent->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::Local);
}

const void UItemConveyorNode::GetItemLocations(TArray<FVector>& OutItemLocations)
{
    for (const FConveyorItem& Item : this->CurrentItems)
    {
        if (UObject* Outer = this->GetOuter())
        {
            if (UActorComponent* Comp = Cast<UActorComponent>(Outer))
            {
                if (AActor* Owner = Comp->GetOwner())
                {
                    OutItemLocations.Add(Owner->GetTransform().TransformPosition(Item.Transform.GetLocation()));
                }
            }
        }
    }
}

void UItemConveyorNode::SetSplineComponent(USplineComponent* InSplineComponent)
{
    this->SplineComponent = InSplineComponent;
}

void UItemConveyorNode::SetStartLocation(const FVector& InStartLocation)
{
    this->StartLocation = InStartLocation;
}

void UItemConveyorNode::SetEndLocation(const FVector& InEndLocation)
{
    this->EndLocation = InEndLocation;
}
