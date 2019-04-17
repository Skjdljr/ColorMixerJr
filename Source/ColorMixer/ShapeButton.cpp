#include "ShapeButton.h"

SShapeButton::SShapeButton() : AdvancedHitTexture(nullptr), AdvancedHitAlpha(1)
{
}

void SShapeButton::SetAdvancedHitTexture(UTexture2D* InTexture)
{
	AdvancedHitTexture = InTexture;
}
void SShapeButton::SetAdvancedHitAlpha(int InAlpha)
{
	AdvancedHitAlpha = InAlpha;
}

FReply SShapeButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!NeedExecuteAction(MyGeometry,MouseEvent))
	{
		return FReply::Unhandled();
	}
	return SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
}

FReply SShapeButton::OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent)
{
	if (!NeedExecuteAction(InMyGeometry, InMouseEvent))
	{
		return FReply::Unhandled();
	}
	return SButton::OnMouseButtonDoubleClick(InMyGeometry, InMouseEvent);
}

FReply SShapeButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (!NeedExecuteAction(MyGeometry, MouseEvent))
	{
		return FReply::Unhandled();
	}
	return SButton::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SShapeButton::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	bool WhatToReturn = NeedExecuteAction(MyGeometry, MouseEvent);
	if (WhatToReturn != bIsHovered)
	{
		bIsHovered = WhatToReturn;
		if (bIsHovered)
		{
			SButton::OnMouseEnter(MyGeometry, MouseEvent);
		}
		else
		{
			SButton::OnMouseLeave(MouseEvent);
		}
	}
	return SButton::OnMouseMove(MyGeometry, MouseEvent);
}
void SShapeButton::OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (AdvancedHitTexture)
	{
		return;
	}
	return SButton::OnMouseEnter(MyGeometry, MouseEvent);
}

void SShapeButton::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	return SButton::OnMouseLeave(MouseEvent);
}

FCursorReply SShapeButton::OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const
{
	if (!bIsHovered)
	{
		return FCursorReply::Unhandled();
	}
	TOptional<EMouseCursor::Type> TheCursor = Cursor.Get();
	return (TheCursor.IsSet()) ? FCursorReply::Cursor(TheCursor.GetValue()) : FCursorReply::Unhandled();
}

TSharedPtr<IToolTip> SShapeButton::GetToolTip()
{
	return (bIsHovered ? SWidget::GetToolTip() : NULL);
}

bool SShapeButton::NeedExecuteAction(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (AdvancedHitTexture == nullptr)
	{
		return true;
	}
	bool WhatToReturn = true;
	FVector2D LocalPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());

	LocalPosition.X = floor(LocalPosition.X);
	LocalPosition.Y = floor(LocalPosition.Y);
	LocalPosition /= MyGeometry.GetLocalSize();

	int ImageWidth = AdvancedHitTexture->PlatformData->SizeX;
	LocalPosition.X *= ImageWidth;
	LocalPosition.Y *= AdvancedHitTexture->PlatformData->SizeY;

	int BufferPosition = (floor(LocalPosition.Y) * ImageWidth) + LocalPosition.X;

	FColor* ImageData = static_cast<FColor*>((AdvancedHitTexture->PlatformData->Mips[0]).BulkData.Lock(LOCK_READ_ONLY));
	if (ImageData == nullptr)
	{
		WhatToReturn = false;
	}
	else
	{
		if (ImageData[BufferPosition].A <= AdvancedHitAlpha)
		{
			WhatToReturn = false;
		}
	}
	AdvancedHitTexture->PlatformData->Mips[0].BulkData.Unlock();

	return WhatToReturn;
}




void UShapeButton::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	(static_cast<SShapeButton*>(MyButton.Get()))->SetAdvancedHitTexture(AdvancedHitTexture);
	(static_cast<SShapeButton*>(MyButton.Get()))->SetAdvancedHitAlpha(AdvancedHitAlpha);
}

TSharedRef<SWidget> UShapeButton::RebuildWidget()
{
	TSharedPtr<SShapeButton> aButton = SNew(SShapeButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.IsFocusable(IsFocusable)
		;

	aButton->SetAdvancedHitTexture(AdvancedHitTexture);
	aButton->SetAdvancedHitAlpha(AdvancedHitAlpha);

	MyButton = aButton;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	return MyButton.ToSharedRef();
}
