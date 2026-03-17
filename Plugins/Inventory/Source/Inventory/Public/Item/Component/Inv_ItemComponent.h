// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/Manifest/Inv_ItemManifest.h"
#include "Inv_ItemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class INVENTORY_API UInv_ItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInv_ItemComponent();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FORCEINLINE FInv_ItemManifest GetItemManifest() const { return ItemManifest; }

	FORCEINLINE FString GetPickUpMessage() const { return PickUpMessage; }

	void PickedUp();

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Inventory")
	void OnPickUp();

private:
	UPROPERTY(EditAnywhere, Replicated, Category="Inventory")
	FInv_ItemManifest ItemManifest;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	FString PickUpMessage;
};
