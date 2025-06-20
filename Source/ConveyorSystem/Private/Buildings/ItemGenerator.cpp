#include "Buildings/ItemGenerator.h"

#include "Graph/Nodes/ItemGeneratorNode.h"

AItemGenerator::AItemGenerator()
{
    this->PrimaryActorTick.bCanEverTick = true;

    this->GraphNodeComponent = CreateDefaultSubobject<UGraphNodeComponent>(TEXT("Graph Node"));
    this->GraphNodeComponent->SetNodeClass(UItemGeneratorNode::StaticClass());

    this->OutItemConnectionComponent = CreateDefaultSubobject<UBuildingConnectionComponent>(TEXT("Output Item Connection"));
    this->OutItemConnectionComponent->SetupAttachment(this->RootComponent);
    this->OutItemConnectionComponent->SetIsInput(false);

    this->ItemToGenerate = nullptr;
}

void AItemGenerator::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (this->ItemToGenerate == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemToGenerate has not been set. No items will be generated."));
        return;
    }

    if (UItemGeneratorNode* ItemGeneratorNode = Cast<UItemGeneratorNode>(this->GraphNodeComponent->GetNode()))
    {
        ItemGeneratorNode->SetItemToGenerate(this->ItemToGenerate);
    }
}
