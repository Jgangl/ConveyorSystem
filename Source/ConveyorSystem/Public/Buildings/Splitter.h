// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

// BEGIN JCore Includes
#include "Building/BuildingConnectionComponent.h"
#include "Graph/GraphNodeComponent.h"
// END JCore Includes

#include "CSBuildable.h"

#include "Splitter.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogSplitter, Log, All)

UCLASS(Abstract, Blueprintable)
class CONVEYORSYSTEM_API ASplitter : public ACSBuildable
{
    GENERATED_BODY()

public:
    ASplitter();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UGraphNodeComponent* GraphNodeComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* EastOutIemConnectionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* WestOutItemConnectionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* InItemConnectionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* NorthOutItemConnectionComponent;
};
