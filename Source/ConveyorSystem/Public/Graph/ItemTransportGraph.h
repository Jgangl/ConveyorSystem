// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

// BEGIN JCore Includes
#include "Graph/GraphBase.h"
// END JCore Includes

#include "Nodes/ItemTransportNode.h"

#include "ItemTransportGraph.generated.h"

UCLASS(BlueprintType)
class CONVEYORSYSTEM_API UItemTransportGraph : public UGraphBase
{
    GENERATED_BODY()
public:
    UItemTransportGraph();

    virtual UNodeBase* AddNode(UNodeBase* NewNode) override;

    virtual void AddEdge(UNodeBase* FromNode, UNodeBase* ToNode) override;

    TArray<UItemTransportNode*> GetChildrenNodes(UNodeBase* ParentNode);

    UFUNCTION(BlueprintCallable)
    TArray<UItemTransportNode*> GetRootNodes();

    UFUNCTION(BlueprintCallable)
    TArray<UItemTransportNode*> GetAllSourceNodes();

    UFUNCTION(BlueprintCallable)
    TArray<UItemTransportNode*> GetTopologicalOrder();
};
