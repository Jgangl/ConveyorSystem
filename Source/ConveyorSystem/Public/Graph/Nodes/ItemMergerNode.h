// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

#include "ItemTransportNode.h"

#include "ItemMergerNode.generated.h"

UCLASS(BlueprintType)
class CONVEYORSYSTEM_API UItemMergerNode : public UItemTransportNode
{
    GENERATED_BODY()
public:
    UItemMergerNode();

    virtual void ProcessItems(float DeltaSeconds);

    virtual bool CanAddItem();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 MaxNumberOfItems;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 RoundRobinIndex;
};
