// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptWorldUserWidget.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"
#include "Kismet/GameplayStatics.h"

void UBuptWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if(!IsValid(AttachedActor))
	{
		RemoveFromParent();

		UE_LOG(LogTemp,Log,TEXT("AttachedActor no longer valid,removing Health Widget."));
		return;
	}
	
	FVector2D ScreenPostion;
	bool bIsOnScreen=UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(),AttachedActor->GetActorLocation()+WorldOffset,ScreenPostion);
	if(bIsOnScreen)
	{
		float Scale=UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPostion/=Scale;

		if(ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPostion);
		}
	}

	if(ParentSizeBox)
	{
		ParentSizeBox->SetVisibility(bIsOnScreen?ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed);
	}
}
