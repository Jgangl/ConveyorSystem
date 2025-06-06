// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

#include "Components/InstancedStaticMeshComponent.h"
#include "Components/SplineComponent.h"

// BEGIN JCore Includes
#include "Building/Buildable.h"
#include "Building/BuildingConnectionComponent.h"
#include "Graph/GraphNodeComponent.h"
// END JCore Includes

#include "ConveyorItem.h"
#include "CSBuildable.h"

#include "Conveyor.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogConveyor, Log, All)

UCLASS(Abstract, Blueprintable)
class CONVEYORSYSTEM_API AConveyor : public ACSBuildable
{
    GENERATED_BODY()

public:
    AConveyor();

    virtual void BeginPlay() override;

    virtual void PostInitializeComponents() override;

    virtual void CompleteBuilding(UBuildingConnectionComponent* FromSnapConnection, UBuildingConnectionComponent* ToSnapConnection) override;

    void SetSplinePointTransform(uint32 SplinePointIndex, const FTransform &InSplineTransform);

    UFUNCTION(BlueprintCallable)
    void CreateBaseInstances(const FVector &Start, const FVector &End);

    void UpdateInstanceTransforms(const float &DeltaTime);

    UFUNCTION(BlueprintCallable)
    FVector GetStartLocation();

    UFUNCTION(BlueprintCallable)
    FVector GetEndLocation();

    UPROPERTY(Category="Options", EditAnywhere, BlueprintReadWrite)
    float Speed;

    UPROPERTY(Category="Options", EditAnywhere, BlueprintReadWrite)
    float DistanceBetweenItems;

    UPROPERTY(Category="Options", EditAnywhere, BlueprintReadWrite)
    float Length;

    UPROPERTY(Category="Options", EditAnywhere, BlueprintReadWrite)
    float BaseLength;

    void OnItemAdded(const FConveyorItem &InItem);

    void OnItemRemoved();

protected:

    virtual void OnConnectionConnected(UBuildingConnectionComponent* FromConnectedConnection,
                                       UBuildingConnectionComponent* ToConnectedConnection) override;

    virtual void OnConnectionDisconnected(UBuildingConnectionComponent* DisconnectedConnection) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UGraphNodeComponent* GraphNodeComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USplineComponent* SplineComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* InItemConnectionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UBuildingConnectionComponent* OutItemConnectionComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInstancedStaticMeshComponent* BaseInstances;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UInstancedStaticMeshComponent* ItemInstances;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient)
    float LastTimeOnScreen;
};
