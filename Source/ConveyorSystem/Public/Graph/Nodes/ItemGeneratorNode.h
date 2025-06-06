// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

#include "ItemTransportNode.h"

#include "ItemGeneratorNode.generated.h"

UCLASS(BlueprintType)
class CONVEYORSYSTEM_API UItemGeneratorNode : public UItemTransportNode
{
    GENERATED_BODY()
public:
    UItemGeneratorNode();

    virtual void ProcessItems(float DeltaSeconds);

    virtual UItemDataAsset* GrabItem(UItemTransportNode* GrabbingNode) override;

    UFUNCTION(BlueprintCallable)
    void SetItemToGenerate(UItemDataAsset* InItem);

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UItemDataAsset* ItemToGenerate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    int32 GenerationRate;

private:
    UPROPERTY(VisibleAnywhere)
    float GenerationTimer;
};
