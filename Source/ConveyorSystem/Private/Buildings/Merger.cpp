#include "Buildings/Merger.h"

#include "Graph/Nodes/ItemMergerNode.h"

AMerger::AMerger()
{
    this->PrimaryActorTick.bCanEverTick = true;

    this->GraphNodeComponent = CreateDefaultSubobject<UGraphNodeComponent>(TEXT("Graph Node"));
    this->GraphNodeComponent->SetNodeClass(UItemMergerNode::StaticClass());

    this->EastInItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("East Input Item Connection"));
    this->EastInItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->EastInItemConnectionComponent->SetIsInput(true);

    this->WestInItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("West Input Item Connection"));
    this->WestInItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->WestInItemConnectionComponent->SetIsInput(true);

    this->SouthInItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("South Input Item Connection"));
    this->SouthInItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->SouthInItemConnectionComponent->SetIsInput(true);

    this->OutItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("Output Item Connection"));
    this->OutItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->OutItemConnectionComponent->SetIsInput(false);
}
