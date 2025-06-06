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

    virtual void CompleteBuilding(UBuildingConnectionComponent* FromSnapConnection, UBuildingConnectionComponent* ToSnapConnection) override;

protected:
    virtual void OnConnectionConnected(UBuildingConnectionComponent* FromConnectedConnection,
                                       UBuildingConnectionComponent* ToConnectedConnection) override;

    virtual void OnConnectionDisconnected(UBuildingConnectionComponent* DisconnectedConnection) override;

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
