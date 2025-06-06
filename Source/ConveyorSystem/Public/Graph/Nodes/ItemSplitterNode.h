// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

#include "ItemTransportNode.h"

#include "ItemSplitterNode.generated.h"

UCLASS(BlueprintType)
class CONVEYORSYSTEM_API UItemSplitterNode : public UItemTransportNode
{
    GENERATED_BODY()
public:
    UItemSplitterNode();

    virtual void ProcessItems(float DeltaSeconds) override;

    virtual bool CanAddItem() override;

    virtual UItemDataAsset* GrabItem(UItemTransportNode* GrabbingNode) override;

    void PushItemsToDestinationNodes();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 MaxNumberOfItems;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 RoundRobinIndex;
};
