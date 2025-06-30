#include "Graph/Nodes/ItemConsumerNode.h"

UItemConsumerNode::UItemConsumerNode()
{
    this->CurrentItems.Empty();
    this->ConsumingRate  = 60.0f; // Items Per Minute
    this->ConsumingTimer = 0.0f;
}

void UItemConsumerNode::ProcessItems(float DeltaSeconds)
{
    this->ConsumingTimer += DeltaSeconds;

    if (this->ConsumingTimer >= (this->ConsumingRate / 60.0f))
    {
        this->ConsumingTimer = 0.0f;

        Super::ProcessItems(DeltaSeconds);
        this->CurrentItems.Empty();
    }
}
