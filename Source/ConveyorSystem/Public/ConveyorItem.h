// Copyright Joshua Gangl. All Rights Reserved.

#pragma once

// BEGIN JCore Includes
#include "Inventory/ItemDataAsset.h"
// END JCore Includes

#include "ConveyorItem.generated.h"

USTRUCT(BlueprintType)
struct FConveyorItem
{
    GENERATED_USTRUCT_BODY()

    FConveyorItem()
    {
        Item      = nullptr;
        Transform = FTransform::Identity;
    }

    FConveyorItem(UItemDataAsset* InItem, const FTransform &InTransform)
    {
        Item      = InItem;
        Transform = InTransform;
    }

    FConveyorItem(UItemDataAsset* InItem)
    {
        Item      = InItem;
        Transform = FTransform::Identity;
    }

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TObjectPtr<UItemDataAsset> Item;

    FTransform Transform;

    FORCEINLINE bool operator==(const FConveyorItem& OtherSegment) const
    {
        return (Item     == OtherSegment.Item &&
                Transform.Equals(OtherSegment.Transform));
    }

    FORCEINLINE bool operator!=(const FConveyorItem& OtherSegment) const
    {
        return (Item     != OtherSegment.Item ||
                !Transform.Equals(OtherSegment.Transform));
    }
};
