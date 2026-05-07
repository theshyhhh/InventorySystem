#include "Widget/Utils/Inv_WidgetUtils.h"

#include "Blueprint/SlateBlueprintLibrary.h"
#include "Components/Widget.h"

int32 UInv_WidgetUtils::GetIndexFromCoordinate(const FIntPoint& Coordinate, const int32 Column)
{
	return Coordinate.X + Coordinate.Y * Column;
}

FIntPoint UInv_WidgetUtils::GetCoordinateFromIndex(const int32 Index, const int32 Column)
{
	return FIntPoint(Index % Column, Index / Column);
}

FVector2D UInv_WidgetUtils::GetWidgetPosition(UWidget* Widget)
{
	const FGeometry& Geometry = Widget->GetCachedGeometry();
	FVector2D PixelPosition;
	FVector2D ViewportPosition;
	USlateBlueprintLibrary::LocalToViewport(Widget, Geometry,/**返回Widget的左上角位置*/ USlateBlueprintLibrary::GetLocalTopLeft(Geometry), PixelPosition,
	                                        ViewportPosition);
	return ViewportPosition;
}

FVector2D UInv_WidgetUtils::GetWidgetSize(UWidget* Widget)
{
	const FGeometry& Geometry = Widget->GetCachedGeometry();
	return Geometry.GetLocalSize();
}

bool UInv_WidgetUtils::IsWithinBounds(const FVector2D& BoundaryPos, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
	return MousePos.X > BoundaryPos.X && MousePos.X < (BoundaryPos.X + WidgetSize.X) && MousePos.Y > BoundaryPos.Y && MousePos.Y < (BoundaryPos.Y +
		WidgetSize.Y);
}

FVector2D UInv_WidgetUtils::GetClampedWidgetPosition(const FVector2D& Boundary, const FVector2D& WidgetSize, const FVector2D& MousePos)
{
	FVector2D ClampedPosition = MousePos;
	if (MousePos.X + WidgetSize.X > Boundary.X)
	{
		ClampedPosition.X = Boundary.X - WidgetSize.X;
	}
	if (MousePos.X < 0)
	{
		ClampedPosition.X = 0;
	}
	if (MousePos.Y + WidgetSize.Y > Boundary.Y)
	{
		ClampedPosition.Y = Boundary.Y - WidgetSize.Y;
	}
	if (MousePos.Y < 0)
	{
		ClampedPosition.Y = 0;
	}
	return ClampedPosition;
}
