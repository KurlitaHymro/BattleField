// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattlefieldCharacterPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"

ABattlefieldCharacterPlayer::ABattlefieldCharacterPlayer()
{
	// Create a springArm attached to the root (capsule)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bDoCollisionTest = false;
	SpringArm->TargetArmLength = 350.f;

	BaseTurnRate = 100.f;
	BaseLookUpRate = 80.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(0.f, 0.f, 30.f));
}

// Called when the game starts or when spawned
void ABattlefieldCharacterPlayer::BeginPlay()
{
	GetState()->InitCharacterState(100, 0, 30, 5, 300);

	Super::BeginPlay();

}

void ABattlefieldCharacterPlayer::CharacterStateUpdate_Implementation(EnumActorStateItem state)
{
	UE_LOG(RunLog, Warning, TEXT("Player::StateUpdate"));
}

void ABattlefieldCharacterPlayer::CreateStateWidget_Implementation()
{
	UE_LOG(LoadLog, Warning, TEXT("Player::CreateStateWidget"));
	UClass* playerStateWidgetClass = LoadClass<UUserWidget>(NULL,
		TEXT("UserWidget'/Game/Widget/UserState.UserState_C'"));
	if (playerStateWidgetClass) {
		StateWidget = CreateWidget<UUserWidget>(GetWorld(), playerStateWidgetClass);
		StateWidget->AddToViewport();
		UCommonInterface::GetDelegation()->playerStateChange.Broadcast(this, EnumActorStateItem::EN_HP);
	} else {
		UE_LOG(LoadLog, Error, TEXT("Not Found Widget Class"));
	}
}
