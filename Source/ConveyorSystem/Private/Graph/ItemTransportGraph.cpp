#include "Graph/ItemTransportGraph.h"

#include "Algo/Reverse.h"

UItemTransportGraph::UItemTransportGraph()
{
    this->bIsDirectedGraph = true;
}

UNodeBase* UItemTransportGraph::AddNode(UNodeBase* NewNode)
{
    return Super::AddNode(NewNode);
}

void UItemTransportGraph::AddEdge(UNodeBase* FromNode, UNodeBase* ToNode)
{
    Super::AddEdge(FromNode, ToNode);

    UItemTransportNode* ItemTransportFromNode = Cast<UItemTransportNode>(FromNode);
    UItemTransportNode* ItemTransportToNode = Cast<UItemTransportNode>(ToNode);

    if (ItemTransportFromNode)
    {
        ItemTransportFromNode->AddDestinationNode(ItemTransportToNode);
    }

    if (ItemTransportToNode)
    {
        ItemTransportToNode->AddSourceNode(ItemTransportFromNode);
    }
}

TArray<UItemTransportNode*> UItemTransportGraph::GetChildrenNodes(UNodeBase* ParentNode)
{
    TArray<UItemTransportNode*> ChildrenNodes;

    for (UEdgeBase* Edge : this->GetEdges())
    {
        if (Edge && Edge->Destination == ParentNode)
        {
            ChildrenNodes.Add(Cast<UItemTransportNode>(Edge->Source));
        }
    }

    return ChildrenNodes;
}

TArray<UItemTransportNode*> UItemTransportGraph::GetRootNodes()
{
    TArray<UItemTransportNode*> RootNodes;

    for (UNodeBase* Node : this->Nodes)
    {
        UItemTransportNode* ItemTransportNode = Cast<UItemTransportNode>(Node);
        if (!ItemTransportNode)
        {
            continue;
        }

        if (this->IsRootNode(Node))
        {
            RootNodes.Add(ItemTransportNode);
        }
    }

    return RootNodes;
}

TArray<UItemTransportNode*> UItemTransportGraph::GetAllSourceNodes()
{
    TArray<UItemTransportNode*> SourceNodes;

    for (UNodeBase* Node : this->Nodes)
    {
        UItemTransportNode* ItemTransportNode = Cast<UItemTransportNode>(Node);
        if (!ItemTransportNode)
        {
            continue;
        }

        if (this->IsSourceNode(Node))
        {
            SourceNodes.Add(ItemTransportNode);
        }
    }

    return SourceNodes;
}

TArray<UItemTransportNode*> UItemTransportGraph::GetTopologicalOrder()
{
    TArray<int32> InDegrees;
    InDegrees.Init(0, this->Nodes.Num());

    // Setup in degrees for each node
    for (int32 i = 0; i < this->Nodes.Num(); i++)
    {
        UItemTransportNode* Node = Cast<UItemTransportNode>(this->Nodes[i]);
        if (!Node)
        {
            continue;
        }

        InDegrees[i] = Node->SourceNodes.Num();
    }

    TQueue<UItemTransportNode*> Queue;
    TArray<UItemTransportNode*> TopOrder;

    // Initially enqueue all source nodes
    for (int32 i = 0; i < InDegrees.Num(); i++)
    {
        if (InDegrees[i] == 0)
        {
            Queue.Enqueue(Cast<UItemTransportNode>(this->Nodes[i]));
        }
    }

    while (!Queue.IsEmpty())
    {
        UItemTransportNode* Node = nullptr;
        Queue.Dequeue(Node);

        TopOrder.Add(Node);

        // Go through each destination node
        for (UItemTransportNode* DestinationNode : Node->DestinationNodes)
        {
            int32 Index = this->Nodes.IndexOfByKey(DestinationNode);

            InDegrees[Index]--;

            if (InDegrees[Index] == 0)
            {
                Queue.Enqueue(DestinationNode);
            }
        }
    }

    Algo::Reverse(TopOrder);

    return TopOrder;
}
