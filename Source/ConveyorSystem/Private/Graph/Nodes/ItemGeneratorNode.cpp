#include "Graph/Nodes/ItemGeneratorNode.h"

UItemGeneratorNode::UItemGeneratorNode()
{
    this->CurrentItems.Empty();
    this->ItemToGenerate = nullptr;
    this->GenerationRate = 60.0f; // Items Per Minute
    this->GenerationTimer = 0.0f;
}

void UItemGeneratorNode::ProcessItems(float DeltaSeconds)
{
    this->GenerationTimer += DeltaSeconds;
}

UItemDataAsset* UItemGeneratorNode::GrabItem(UItemTransportNode* GrabbingNode)
{
    if (this->DestinationNodes.IsEmpty())
    {
        return nullptr;
    }

    if (this->GenerationTimer >= (this->GenerationRate / 60.0f))
    {
        this->GenerationTimer = 0.0f;
        return this->ItemToGenerate;
    }

    return nullptr;
}

void UItemGeneratorNode::SetItemToGenerate(UItemDataAsset* InItem)
{
    this->ItemToGenerate = InItem;
}
