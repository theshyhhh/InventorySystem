#include "Item/Component/Inv_HighlightableStaticMeshComponent.h"

void UInv_HighlightableStaticMeshComponent::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void UInv_HighlightableStaticMeshComponent::Unhighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
