// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

#include "Graph/GraphDebugger.h"

#include "ItemTransportGraphDebugger.generated.h"

UCLASS(BlueprintType)
class CONVEYORSYSTEM_API AItemTransportGraphDebugger: public AGraphDebugger
{
    GENERATED_BODY()

public:
    AItemTransportGraphDebugger();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    virtual void DrawGraph() override;

    void DrawUpdateOrder();

    void DrawItemLocations();

    void DrawSplineLocations();

protected:

    UPROPERTY(EditAnywhere, meta=(EditCondition="bEnabled"))
    bool bDrawUpdateOrder;

    UPROPERTY(EditAnywhere, meta=(EditCondition="bEnabled"))
    bool bDrawItemLocations;

    UPROPERTY(EditAnywhere, meta=(EditCondition="bEnabled"))
    bool bDrawSplineLocations;

    UPROPERTY(EditAnywhere)
    float DistanceBetweenSplineDebugSpheres;
};
