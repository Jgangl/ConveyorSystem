#include "Buildings/ItemConsumer.h"

#include "Graph/Nodes/ItemConsumerNode.h"

AItemConsumer::AItemConsumer()
{
    this->PrimaryActorTick.bCanEverTick = true;

    this->GraphNodeComponent = CreateDefaultSubobject<UGraphNodeComponent>(TEXT("Graph Node"));
    this->GraphNodeComponent->SetNodeClass(UItemConsumerNode::StaticClass());

    this->InItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("In Item Connection"));
    this->InItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->InItemConnectionComponent->SetIsInput(true);
}
