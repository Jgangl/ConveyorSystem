#include "Buildings/Splitter.h"

#include "Graph/Nodes/ItemSplitterNode.h"

ASplitter::ASplitter()
{
    this->PrimaryActorTick.bCanEverTick = true;

    this->GraphNodeComponent = CreateDefaultSubobject<UGraphNodeComponent>(TEXT("Graph Node"));
    this->GraphNodeComponent->SetNodeClass(UItemSplitterNode::StaticClass());

    this->EastOutIemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("East Output Item Connection"));
    this->EastOutIemConnectionComponent->SetupAttachment(this->RootComponent);
    this->EastOutIemConnectionComponent->SetIsInput(false);

    this->WestOutItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("West Output Item Connection"));
    this->WestOutItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->WestOutItemConnectionComponent->SetIsInput(false);

    this->NorthOutItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("North Output Item Connection"));
    this->NorthOutItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->NorthOutItemConnectionComponent->SetIsInput(false);

    this->InItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("Input Item Connection"));
    this->InItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->InItemConnectionComponent->SetIsInput(true);
}
