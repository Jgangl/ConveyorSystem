#include "Graph/Nodes/ItemTransportNode.h"

UItemTransportNode::UItemTransportNode()
{
    this->CurrentItems.Empty();
}

void UItemTransportNode::AddSourceNode(UItemTransportNode* InSourceNode)
{
    this->SourceNodes.Add(InSourceNode);
}

void UItemTransportNode::AddDestinationNode(UItemTransportNode* InDestinationNode)
{
    this->DestinationNodes.Add(InDestinationNode);
}

void UItemTransportNode::UpdateItems(float DeltaSeconds)
{
}

void UItemTransportNode::ProcessItems(float DeltaSeconds)
{
    if (this->SourceNodes.IsEmpty())
    {
        return;
    }

    // Only try to pull an item from the source if we can add it
    if (!this->CanAddItem())
    {
        return;
    }

    UItemTransportNode* SourceNode = this->SourceNodes.Last();

    if (!SourceNode)
    {
        return;
    }

    UItemDataAsset* OutItem = SourceNode->GrabItem(this);

    if (!OutItem)
    {
        return;
    }

    this->AddItem(OutItem);
}

UItemDataAsset* UItemTransportNode::GrabItem(UItemTransportNode* GrabbingNode)
{
    return this->TryRemoveItemFromEnd();
}

bool UItemTransportNode::CanGrabItem()
{
    if (this->CurrentItems.Num() == 0)
    {
        return false;
    }

    return true;
}

void UItemTransportNode::AddItem(UItemDataAsset* InItem)
{
    if (!InItem)
    {
        UE_LOG(LogTemp, Error, TEXT("%hs : InItem is nullptr"), __FUNCTION__);
        return;
    }

    const FConveyorItem NewConveyorItem = FConveyorItem(InItem);

    this->CurrentItems.Add(NewConveyorItem);
}

bool UItemTransportNode::CanAddItem()
{
    if (this->CurrentItems.Num() == 0)
    {
        return true;
    }

    return true;
}

UItemDataAsset* UItemTransportNode::TryRemoveItemFromEnd()
{
    if (this->CurrentItems.Num() == 0) return nullptr;

    UItemDataAsset* ItemToRemove = this->CurrentItems[0].Item;

    // Remove the first item
    // TODO: Should the CurrentItems Array be a queue instead?
    this->CurrentItems.RemoveAt(0);

    return ItemToRemove;
}

const TArray<FConveyorItem>& UItemTransportNode::GetItems() const
{
    return this->CurrentItems;
}
