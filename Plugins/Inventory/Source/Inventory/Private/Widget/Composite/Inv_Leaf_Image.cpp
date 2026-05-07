#include "Widget/Composite/Inv_Leaf_Image.h"

#include "Components/Image.h"
#include "Components/SizeBox.h"

void UInv_Leaf_Image::SetImage(UTexture2D* Texture) const
{
	Image->SetBrushFromTexture(Texture);
}

void UInv_Leaf_Image::SetImageSize(const FVector2D& Size) const
{
	Image->SetDesiredSizeOverride(Size);
	SizeBox->SetWidthOverride(Size.X);
	SizeBox->SetHeightOverride(Size.Y);
}

void UInv_Leaf_Image::SetBoxSize(const FVector2D& Size) const
{
}
