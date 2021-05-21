// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattlefieldCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Component/ActorState.h"
#include "Component/CharacterAction.h"
#include "Widget/UserWidgetBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABattlefieldCharacterBase::ABattlefieldCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	bIsValid = false;
	InMotionRotatorChangeRate = 1.0f;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 90.f);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	ComponentInit();
	bIsValid = true;
}

// Called when the game starts or when spawned
void ABattlefieldCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	BeginPlayLoad();
}

// Called every frame
void ABattlefieldCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABattlefieldCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float ABattlefieldCharacterBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	GetState()->HpChange(-Damage);
	CharacterStateUpdate(EnumActorStateItem::EN_HP);
	if (GetState()->GetStateItem(EnumActorStateItem::EN_HP) <= 0) {
		bIsValid = false;
		PawnDead.Broadcast();
	}

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ABattlefieldCharacterBase::HitActor(AActor* targetActor)
{
	UGameplayStatics::ApplyDamage(targetActor,
		CalDamage(),
		this->GetController(),
		this,
		DamageTypeClass);
	return;
}

UStaticMeshComponent* ABattlefieldCharacterBase::GetWeaponMesh(int weaponId)
{
	if (Weapon.IsValidIndex(weaponId)) {
		return Weapon[weaponId];
	} else {
		UE_LOG(RunLog, Error, TEXT("Weapon Index Error"));
		return nullptr;
	}
}

UActorState* ABattlefieldCharacterBase::GetState()
{
	if (State) {
		return State;
	} else {
		UE_LOG(LoadLog, Error, TEXT("CharacterBase No State"));
		return nullptr;
	}
}

UCharacterAction* ABattlefieldCharacterBase::GetAction()
{
	if (Action) {
		return Action;
	} else {
		UE_LOG(LoadLog, Error, TEXT("CharacterBase No Action"));
		return nullptr;
	}
	return nullptr;
}

void ABattlefieldCharacterBase::ComponentInit()
{
	/* step1 : 创建组件 */
	/* 1.1 状态管理组件 */
	State = CreateDefaultSubobject<UActorState>(TEXT("State"));
	if (State) {
		RunSpeedChangeValue = 300;
	} else {
		bIsValid = false; // 组件内存申请失败时角色不可用。
		return;
	}

	/* 1.2 动作管理组件 */
	Action = CreateDefaultSubobject<UCharacterAction>(TEXT("Action"));
	if (Action) {
		
	} else {
		bIsValid = false; // 组件内存申请失败时角色不可用。
		return;
	}

	// TODO: 做一个装备的表，先读取衣服，生成新的SkMesh；再读武器，绑到对应插槽上或隐藏。
	/* 1.3 Mesh组件 */
	if (GetMesh()) {
	} else {
		bIsValid = false; // 组件内存申请失败时角色不可用。
		return;
	}
	USkeletalMesh* skMesh = LoadObject<USkeletalMesh>(NULL,
		TEXT("SkeletalMesh'/Game/SkeletalMesh/SK_Mesh_Default.SK_Mesh_Default'"));
	if (skMesh) {
		FTransform tBody = UKismetMathLibrary::MakeTransform(
			FVector(0.f, 0.f, -90.f),
			FRotator(0.f, -90.f, 0.f),
			FVector(1.f, 1.f, 1.f));
		GetMesh()->SetSkeletalMesh(skMesh);
		GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetMesh()->SetRelativeTransform(tBody);
	} else {
		UE_LOG(LoadLog, Error, TEXT("Load SkeletalMesh Error"));
	}

	/* 1.4 武器管理组件 */
	for (int weaponId = 0; weaponId < 2; weaponId++) {
		UStaticMeshComponent* stMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(UCommonInterface::CombinFNameId("Weapon", weaponId));
		if (stMeshComp) {
			stMeshComp->SetupAttachment(GetMesh(), UCommonInterface::CombinFNameId("Eqip", weaponId));
		} else {
			bIsValid = false; // 组件内存申请失败时角色不可用。
			return;
		}
		UStaticMesh* weapon;
		FTransform tWeapon;
		if (0 == weaponId) {
			 weapon = LoadObject<UStaticMesh>(NULL,
				TEXT("StaticMesh'/Game/StaticMesh/Weapon/Heavy/HeavyLongAxe/HeavyLongAxe.HeavyLongAxe'"));
			 tWeapon = UKismetMathLibrary::MakeTransform(
				 FVector(0.f, 0.f, 0.f),
				 FRotator(0.f, 0.f, 0.f),
				 FVector(1.0f, 1.0f, 0.7f));
		} else {
			weapon = LoadObject<UStaticMesh>(NULL,
				TEXT("StaticMesh'/Game/StaticMesh/Weapon/Shield/IronBuckler/IronBuckler.IronBuckler'"));
			tWeapon = UKismetMathLibrary::MakeTransform(
				FVector(0.f, 3.f, -3.f),
				FRotator(0.f, 0.f, 0.f),
				FVector(0.7f, 0.7f, 0.8f));
		}

		if (weapon) {
			stMeshComp->SetStaticMesh(weapon);
			stMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			stMeshComp->SetRelativeTransform(tWeapon);
		} else {
			UE_LOG(LoadLog, Error, TEXT("Load Weapon Error"));
		}
		Weapon.Emplace(stMeshComp);
	}

	/* 1.5 角色状态控件组件 */
	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	WidgetComp->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	/* 1.6 动画蓝图 */
	UAnimBlueprint* abp = LoadObject<UAnimBlueprint>(NULL,
		TEXT("AnimBlueprint'/Game/SkeletalMesh/SK_AnimBP_Default.SK_AnimBP_Default'"));
	if (abp) {
		GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		GetMesh()->SetAnimInstanceClass(abp->GetAnimBlueprintGeneratedClass());
	} else {
		UE_LOG(LoadLog, Error, TEXT("Load AnimBP Error"));
	}
}

void ABattlefieldCharacterBase::BeginPlayLoad()
{
	/* step2 : 加载内容 */
	/* 2.1 虚控件蓝图 */
	CreateStateWidget();

	/* 2.2 启用角色动作 */
	if (bIsValid) {
		bIsInMotion = false;
	}
}

float ABattlefieldCharacterBase::CalDamage()
{
	int damage = State->GetStateItem(EnumActorStateItem::EN_ATK);
	int reduce = State->GetStateItem(EnumActorStateItem::EN_DEF);
	return static_cast<float>(damage - reduce);
}

void ABattlefieldCharacterBase::CharacterStateUpdate_Implementation(EnumActorStateItem state)
{
	if (WidgetComp) {
		UUserWidget* widget = WidgetComp->GetUserWidgetObject();
		UUserWidgetBase* base = Cast<UUserWidgetBase>(widget);
		if (base) {
			base->StateUpdate(this, EnumActorStateItem::EN_HP);
		} else {
			UE_LOG(RunLog, Error, TEXT("StateUpdate Widget Error"));
		}
	}
}

void ABattlefieldCharacterBase::CreateStateWidget_Implementation()
{
	UClass* characterStateWidgetClass = LoadClass<UUserWidget>(NULL,
		TEXT("UserWidget'/Game/Widget/CharacterState.CharacterState_C'"));
	if (characterStateWidgetClass) {
		WidgetComp->SetWidgetClass(characterStateWidgetClass);
		WidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
		FTransform tWidget = UKismetMathLibrary::MakeTransform(
			FVector(0.f, 0.f, 100.f),
			FRotator(0.f, 0.f, 0.f),
			FVector(2.f, 2.f, 2.f));
		WidgetComp->SetRelativeTransform(tWidget);
	} else {
		UE_LOG(LoadLog, Error, TEXT("Not Found Widget Class"));
	}
}

FRotator ABattlefieldCharacterBase::GetExceptRotatorInMotion_Implementation()
{
	return GetActorRotation();
}

void ABattlefieldCharacterBase::AxisInput(EnumCharacterAxisAction axis, float value)
{
	switch (axis)
	{
	case EnumCharacterAxisAction::EN_MOVE_Y:
		MoveForward(value);
		break;
	case EnumCharacterAxisAction::EN_MOVE_X:
		MoveRight(value);
		break;
	case EnumCharacterAxisAction::EN_LOOK_Y:
		LookUpAtRate(value);
		break;
	case EnumCharacterAxisAction::EN_LOOK_X:
		TurnAtRate(value);
		break;
	}
}

void ABattlefieldCharacterBase::OnceInput(EnumCharacterOnceAction action, float value)
{
	switch (action)
	{
	case EnumCharacterOnceAction::EN_ATTACK:
		if (!bIsInMotion || bRcvNextInput) {
			MainNormalAttack();
		}
		break;
	case EnumCharacterOnceAction::EN_JUMP:
		if (!bIsInMotion) {
			JumpStarted();
		}
		break;
	case EnumCharacterOnceAction::EN_JUMP_STOP:
		JumpStopped();
		break;
	case EnumCharacterOnceAction::EN_SPEED_PLUS:
		ChangeSpeed(RunSpeedChangeValue);
		break;
	case EnumCharacterOnceAction::EN_SPEED_SUB:
		ChangeSpeed(-RunSpeedChangeValue);
		break;
	}
}

void ABattlefieldCharacterBase::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ABattlefieldCharacterBase::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ABattlefieldCharacterBase::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ABattlefieldCharacterBase::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ABattlefieldCharacterBase::JumpStarted()
{
	Jump();
}

void ABattlefieldCharacterBase::JumpStopped()
{
	StopJumping();
}

void ABattlefieldCharacterBase::ChangeSpeed(int spd)
{
	GetState()->SpeedChange(spd);
}

void ABattlefieldCharacterBase::ResetSpeed()
{
	GetState()->ResetSpeed();
}

void ABattlefieldCharacterBase::MainNormalAttack()
{
	Action->MainNormalAttack();
}

void ABattlefieldCharacterBase::Dead()
{
	UAnimMontage* montage = LoadObject<UAnimMontage>(NULL,
		TEXT("AnimBlueprint'/Game/Animations/Montage/DeadMontage.DeadMontage'"));
	if (montage) {
		PlayAnimMontage(montage, 1.4f);
	} else {
		UE_LOG(LoadLog, Error, TEXT("Load Montage Error"));
	}
}