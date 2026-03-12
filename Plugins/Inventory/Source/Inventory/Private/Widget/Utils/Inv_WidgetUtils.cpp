#include "Widget/Utils/Inv_WidgetUtils.h"

int32 UInv_WidgetUtils::GetIndexFromPosition(const FIntPoint& Position, const int32 Column)
{
	return Position.Y + Position.X * Column;
}

FIntPoint UInv_WidgetUtils::GetPositionFromIndex(const int32 Index, const int32 Column)
{
	return FIntPoint(Index % Column, Index / Column);
}
