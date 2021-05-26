// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattlefieldCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/KismetMathLibrary.h"

ABattlefieldCharacterPlayer::ABattlefieldCharacterPlayer()
{
	// Create a springArm attached to the root (capsule)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 350.f; 

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
	
	BaseTurnRate = 100.f;
	BaseLookUpRate = 80.f;
}

// Called when the game starts or when spawned
void ABattlefieldCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	GetState()->InitCharacterState(10000, 100, 500, 100, 300);
}

void ABattlefieldCharacterPlayer::CharacterStateUpdate_Implementation(EnumActorStateItem state)
{
	UCommonInterface::GetDelegation()->PlayerStateChange.Broadcast(this, EnumActorStateItem::EN_HP);
}

void ABattlefieldCharacterPlayer::CreateStateWidget_Implementation()
{
	UClass* playerStateWidgetClass = LoadClass<UUserWidget>(NULL,
		TEXT("UserWidget'/Game/Widget/UserState.UserState_C'"));
	if (playerStateWidgetClass) {
		StateWidget = CreateWidget<UUserWidget>(GetWorld(), playerStateWidgetClass);
		StateWidget->AddToViewport();
		UCommonInterface::GetDelegation()->PlayerStateChange.Broadcast(this, EnumActorStateItem::EN_HP);
	} else {
		UE_LOG(LoadLog, Error, TEXT("Not Found Widget Class"));
	}
}

FRotator ABattlefieldCharacterPlayer::GetExceptRotatorInMotion_Implementation()
{
	// 获取Movement组件中保存的轴输入的最终移动方向作为动作中欲面向Rotation
	const FVector Direction = GetCharacterMovement()->GetLastInputVector();
	return UKismetMathLibrary::Conv_VectorToRotator(Direction);
}
