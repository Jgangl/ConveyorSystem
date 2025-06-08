// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

// BEGIN JCore Includes
#include "Building/BuildingConnectionComponent.h"
#include "Graph/GraphNodeComponent.h"
#include "Inventory/ItemDataAsset.h"
// END JCore Includes

#include "CSBuildable.h"

#include "ItemGenerator.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogItemGenerator, Log, All)

UCLASS(Abstract, Blueprintable)
class CONVEYORSYSTEM_API AItemGenerator : public ACSBuildable
{
    GENERATED_BODY()

public:
    AItemGenerator();

    virtual void PostInitializeComponents() override;

protected:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UItemDataAsset* ItemToGenerate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UGraphNodeComponent* GraphNodeComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* OutItemConnectionComponent;
};
