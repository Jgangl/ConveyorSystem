// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

// BEGIN JCore Includes
#include "Building/BuildingConnectionComponent.h"
#include "Graph/GraphNodeComponent.h"
#include "Inventory/ItemDataAsset.h"
// END JCore Includes

#include "CSBuildable.h"

#include "ItemConsumer.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogItemConsumer, Log, All)

UCLASS(Abstract, Blueprintable)
class CONVEYORSYSTEM_API AItemConsumer : public ACSBuildable
{
    GENERATED_BODY()

public:
    AItemConsumer();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UGraphNodeComponent* GraphNodeComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* InItemConnectionComponent;
};
