// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

// BEGIN JCore Includes
#include "Building/Buildable.h"
// END JCore Includes

#include "CSBuildable.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogCSBuildable, Log, All)

UCLASS(Abstract, Blueprintable)
class CONVEYORSYSTEM_API ACSBuildable : public ABuildable
{
    GENERATED_BODY()

public:
    ACSBuildable();

    virtual void BeginPlay() override;

    virtual void CompleteBuilding(UBuildingConnectionComponent* FromSnapConnection, UBuildingConnectionComponent* ToSnapConnection) override;

    virtual void AddConnection(UBuildingConnectionComponent* FromConnection, UBuildingConnectionComponent* ToConnection);

    void UpdateGraphConnections();
};
