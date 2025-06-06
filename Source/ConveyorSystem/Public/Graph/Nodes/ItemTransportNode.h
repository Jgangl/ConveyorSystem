// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

// BEGIN JCore Includes
#include "Graph/NodeBase.h"
// END JCore Includes

#include "ConveyorItem.h"

#include "ItemTransportNode.generated.h"

UCLASS(BlueprintType)
class CONVEYORSYSTEM_API UItemTransportNode : public UNodeBase
{
    GENERATED_BODY()
public:
    UItemTransportNode();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<UItemTransportNode*> SourceNodes;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<UItemTransportNode*> DestinationNodes;

    void AddSourceNode(UItemTransportNode* InSourceNode);

    void AddDestinationNode(UItemTransportNode* InDestinationNode);

    virtual void UpdateItems(float DeltaSeconds);

    virtual void ProcessItems(float DeltaSeconds);

    virtual UItemDataAsset* GrabItem(UItemTransportNode* GrabbingNode);

    UFUNCTION(BlueprintCallable)
    virtual bool CanGrabItem();

    UFUNCTION(CallInEditor, BlueprintCallable)
    virtual void AddItem(UItemDataAsset* InItem);

    UFUNCTION(BlueprintCallable)
    virtual bool CanAddItem();

    UFUNCTION(CallInEditor, Category="Options", BlueprintCallable)
    virtual UItemDataAsset* TryRemoveItemFromEnd();

    UFUNCTION(BlueprintCallable)
    const TArray<FConveyorItem>& GetItems() const;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    TArray<FConveyorItem> CurrentItems;
};
