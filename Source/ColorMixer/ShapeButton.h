#pragma once

#include "ColorMixer.h"
#include "Components/Button.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "ShapeButton.generated.h"


class UTexture2D;

class COLORMIXER_API SShapeButton : public SButton
{
public:

	UTexture2D * AdvancedHitTexture;
	int AdvancedHitAlpha;

	SShapeButton();

	virtual void SetAdvancedHitTexture(UTexture2D* InTexture);
	virtual void SetAdvancedHitAlpha(int InAlpha);
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonDoubleClick(const FGeometry& InMyGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseEnter(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual FCursorReply OnCursorQuery(const FGeometry& MyGeometry, const FPointerEvent& CursorEvent) const override;
	virtual TSharedPtr<IToolTip> GetToolTip() override;
private:
	virtual bool NeedExecuteAction(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
};


/**
* Unusual shape button
* To define a shape, use a texture with an alpha channel
 */
UCLASS()
class COLORMIXER_API UShapeButton : public UButton
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AdvancedHitTest")
	UTexture2D* AdvancedHitTexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AdvancedHitTest", meta = (ClampMin = "0.0", ClampMax = "255.0", UIMin = "0.0", UIMax = "255.0"))
	int AdvancedHitAlpha;

	UFUNCTION(BlueprintCallable, Category = "AdvancedHitTest")
		void SetAdvancedHitTexture(UTexture2D* InTexture)
	{
		AdvancedHitTexture = InTexture;
		if (MyButton.IsValid()) (static_cast<SShapeButton*>(MyButton.Get()))->SetAdvancedHitTexture(AdvancedHitTexture);
	}

	UFUNCTION(BlueprintCallable, Category = "AdvancedHitTest")
	void SetAdvancedHitAlpha(int InAlpha)
	{
		AdvancedHitAlpha = InAlpha;
		if (MyButton.IsValid()) (static_cast<SShapeButton*>(MyButton.Get()))->SetAdvancedHitAlpha(AdvancedHitAlpha);
	}

	UShapeButton(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer), AdvancedHitTexture(nullptr), AdvancedHitAlpha(1) {}

	virtual void SynchronizeProperties() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
};
