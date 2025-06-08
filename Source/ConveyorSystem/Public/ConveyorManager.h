// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

#include "Buildings/Conveyor.h"
#include "Graph/ItemTransportGraph.h"

#include "ConveyorManager.generated.h"

DECLARE_LOG_CATEGORY_CLASS(LogConveyorManager, Log, All)

UCLASS(Blueprintable)
class CONVEYORSYSTEM_API AConveyorManager : public AActor
{
    GENERATED_BODY()

public:
    AConveyorManager();

    virtual void BeginPlay() override;

    virtual void Tick(float DeltaSeconds) override;

    virtual void PostInitializeComponents() override;

    void UpdateAllItemTransports(const TArray<UItemTransportNode*>& Order, float DeltaSeconds);

    void ProcessAllItemTransports(const TArray<UItemTransportNode*>& Order, float DeltaSeconds);

    void UpdateAllConveyors(float DeltaSeconds);

    UItemTransportGraph* GetGraph();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    UItemTransportGraph* ItemTransportGraph;

    UFUNCTION(BlueprintCallable)
    void AddItemToConveyor(UItemDataAsset* InItem);

    UFUNCTION(BlueprintCallable)
    void AddItemToAllSourceConveyors(UItemDataAsset* InItem);

    UFUNCTION(BlueprintCallable)
    static AConveyorManager* Get();

protected:

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<AConveyor*> Conveyors;

private:
    static TWeakObjectPtr<AConveyorManager> Instance;
};
