#include "Player/Inv_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Inventory.h"
#include "Interaction/Inv_HighlightInterface.h"
#include "Item/Component/Inv_ItemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/HUD/Inv_HUDWidget.h"

AInv_PlayerController::AInv_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AInv_PlayerController::BeginPlay()
{
	Super::BeginPlay();
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(InputSubsystem);
	InputSubsystem->AddMappingContext(DefaultIMC, 0);
	CreateHUDWidget();
}

void AInv_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	UEnhancedInputComponent* EnhancedInputComp = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComp->BindAction(PrimaryInteractionAction, ETriggerEvent::Started, this, &AInv_PlayerController::PrimaryInteract);
}

void AInv_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TraceForItem();
}

void AInv_PlayerController::PrimaryInteract()
{
	UE_LOG(LogInventory, Log, TEXT("Primary interact"));
}

void AInv_PlayerController::CreateHUDWidget()
{
	if (!IsLocalController())return;
	HUDWidget = CreateWidget<UInv_HUDWidget>(this, HUDWidgetClass);
	if (IsValid(HUDWidget))
	{
		HUDWidget->AddToViewport();
	}
}

void AInv_PlayerController::TraceForItem()
{
	if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport))return;
	FVector2D ViewportSize;
	GEngine->GameViewport->GetViewportSize(ViewportSize);
	//屏幕中心位置
	FVector2D ViewportCenter = ViewportSize / 2;
	FVector TraceStart;
	FVector Forward;
	//将屏幕坐标转换为世界坐标
	if (!UGameplayStatics::DeprojectScreenToWorld(this, ViewportCenter, TraceStart, Forward))return;
	FVector TraceEnd = TraceStart + Forward * TraceLength;
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ItemTraceChannel);
	PreviousActor = CurrentActor;
	CurrentActor = HitResult.GetActor();
	//如果当前指向的Actor为空，则隐藏拾取信息
	if (!CurrentActor.IsValid() && IsValid(HUDWidget))HUDWidget->HidePickUpMessage();
	if (CurrentActor == PreviousActor)return;
	if (CurrentActor.IsValid())
	{
		UE_LOG(LogInventory, Warning, TEXT("Current Actor %s"), *CurrentActor->GetName());
		UInv_ItemComponent* ItemComp = CurrentActor->FindComponentByClass<UInv_ItemComponent>();
		//显示拾取信息
		if (IsValid(ItemComp) && IsValid(HUDWidget))HUDWidget->ShowPickUpMessage(ItemComp->GetPickUpMessage());
		//高亮当前检测到的Actor
		if (UActorComponent* HighlightableComp = CurrentActor->FindComponentByInterface(UInv_HighlightInterface::StaticClass());
			IsValid(HighlightableComp))
		{
			IInv_HighlightInterface::Execute_Highlight(HighlightableComp);
		}
	}
	if (PreviousActor.IsValid())
	{
		UE_LOG(LogInventory, Warning, TEXT("Previous Actor %s"), *PreviousActor->GetName());
		//取消上一个检测到的Actor的高光
		if (UActorComponent* HighlightableComp = PreviousActor->FindComponentByInterface(UInv_HighlightInterface::StaticClass());
			IsValid(HighlightableComp))
		{
			IInv_HighlightInterface::Execute_Unhighlight(HighlightableComp);
		}
	}
}
