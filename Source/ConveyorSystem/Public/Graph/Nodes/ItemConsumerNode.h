// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

#include "ItemTransportNode.h"

#include "ItemConsumerNode.generated.h"

UCLASS(BlueprintType)
class CONVEYORSYSTEM_API UItemConsumerNode : public UItemTransportNode
{
    GENERATED_BODY()
public:
    UItemConsumerNode();

    virtual void ProcessItems(float DeltaSeconds);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 ConsumingRate;

private:
    UPROPERTY(VisibleAnywhere)
    float ConsumingTimer;
};
