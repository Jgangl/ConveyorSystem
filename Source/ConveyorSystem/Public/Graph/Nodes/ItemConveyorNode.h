// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

#include "Components/SplineComponent.h"

#include "ConveyorItem.h"
#include "ItemTransportNode.h"

#include "ItemConveyorNode.generated.h"

DECLARE_DELEGATE_OneParam(FOnConveyorItemAdded, const FConveyorItem&)
DECLARE_DELEGATE(FOnConveyorItemRemoved)

UCLASS(BlueprintType)
class CONVEYORSYSTEM_API UItemConveyorNode : public UItemTransportNode
{
    GENERATED_BODY()
public:
    UItemConveyorNode();

    virtual void UpdateItems(float DeltaSeconds) override;

    virtual void AddItem(UItemDataAsset* InItem) override;

    virtual bool CanAddItem() override;

    virtual bool CanGrabItem() override;

    virtual UItemDataAsset* TryRemoveItemFromEnd() override;

    UFUNCTION(BlueprintCallable)
    FVector GetStartLocation();

    UFUNCTION(BlueprintCallable)
    FVector GetEndLocation();

    UFUNCTION(BlueprintCallable)
    const void GetItemLocations(TArray<FVector>& OutItemLocations);

    void SetSplineComponent(USplineComponent* InSplineComponent);

    void SetStartLocation(const FVector& InStartLocation);

    void SetEndLocation(const FVector& InEndLocation);

    UPROPERTY(Category="Options", EditAnywhere, BlueprintReadWrite)
    float Speed;

    UPROPERTY(Category="Options", EditAnywhere, BlueprintReadWrite)
    float DistanceBetweenItems;

    UPROPERTY(Category="Options", EditAnywhere, BlueprintReadWrite)
    float Length;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector StartLocation;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector EndLocation;

    FOnConveyorItemAdded OnItemAdded;

    FOnConveyorItemRemoved OnItemRemoved;

protected:

    UPROPERTY()
    USplineComponent* SplineComponent;
};
