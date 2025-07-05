// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

// BEGIN JCore Includes
#include "Building/BuildingConnectionComponent.h"
#include "Graph/GraphNodeComponent.h"
// END JCore Includes

#include "CSBuildable.h"

#include "Merger.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogMerger, Log, All)

UCLASS(Abstract, Blueprintable)
class CONVEYORSYSTEM_API AMerger : public ACSBuildable
{
    GENERATED_BODY()

public:
    AMerger();

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UGraphNodeComponent* GraphNodeComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* EastInItemConnectionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* WestInItemConnectionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* SouthInItemConnectionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* OutItemConnectionComponent;
};
