#include "Graph/Nodes/ItemSplitterNode.h"

UItemSplitterNode::UItemSplitterNode()
{
    this->CurrentItems.Empty();
    this->MaxNumberOfItems = 1;
    this->RoundRobinIndex = 0;
}

void UItemSplitterNode::ProcessItems(float DeltaSeconds)
{
    // We need to push items to destination nodes before pulling from the source node
    this->PushItemsToDestinationNodes();

    // This will pull in items from the source node
    Super::ProcessItems(DeltaSeconds);
}

bool UItemSplitterNode::CanAddItem()
{
    if (this->CurrentItems.Num() >= this->MaxNumberOfItems)
    {
        return false;
    }

    return true;
}

UItemDataAsset* UItemSplitterNode::GrabItem(UItemTransportNode* GrabbingNode)
{
    // Splitters basically "push" items and we don't want to allow destination nodes to grab items themselves
    return nullptr;
}

void UItemSplitterNode::PushItemsToDestinationNodes()
{
    if (this->DestinationNodes.IsEmpty())
    {
        return;
    }

    if (!this->CanGrabItem())
    {
        return;
    }

    int32 OriginalIndex = this->RoundRobinIndex;

    // We need to grab from the first source node in the round-robin order that has an item ready to be grabbed
    do
    {
        UItemTransportNode* DestinationNode = this->DestinationNodes[this->RoundRobinIndex];
        if (!DestinationNode)
        {
            continue;
        }

        // If the destination node can take an item, then give it
        if (DestinationNode->CanAddItem())
        {
            DestinationNode->AddItem(Super::GrabItem(DestinationNode));
            this->RoundRobinIndex = (this->RoundRobinIndex + 1) % this->DestinationNodes.Num();
            break;
        }

        // Increment and wrap the round-robin index
        this->RoundRobinIndex = (this->RoundRobinIndex + 1) % this->DestinationNodes.Num();
    }
    while (OriginalIndex != this->RoundRobinIndex);
}
