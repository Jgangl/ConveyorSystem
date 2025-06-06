#include "Graph/Nodes/ItemMergerNode.h"

UItemMergerNode::UItemMergerNode()
{
    this->CurrentItems.Empty();
    this->MaxNumberOfItems = 1;
    this->RoundRobinIndex = 0;
}

void UItemMergerNode::ProcessItems(float DeltaSeconds)
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

    if (!this->SourceNodes.IsValidIndex(this->RoundRobinIndex))
    {
        return;
    }

    int32 OriginalIndex = this->RoundRobinIndex;

    // We need to grab from the first source node in the round-robin order that has an item ready to be grabbed
    do
    {
        UItemTransportNode* SourceNode = this->SourceNodes[this->RoundRobinIndex];
        if (!SourceNode)
        {
            continue;
        }

        if (SourceNode->CanGrabItem())
        {
            // Grab an item from the current source node
            this->AddItem(SourceNode->GrabItem(this));
            this->RoundRobinIndex = (this->RoundRobinIndex + 1) % this->SourceNodes.Num();
            break;
        }

        // Increment and wrap the round-robin index
        this->RoundRobinIndex = (this->RoundRobinIndex + 1) % this->SourceNodes.Num();
    }
    while (OriginalIndex != this->RoundRobinIndex);
}

bool UItemMergerNode::CanAddItem()
{
    if (this->CurrentItems.Num() >= this->MaxNumberOfItems)
    {
        return false;
    }

    return true;
}
