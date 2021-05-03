// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BattlefieldCharacterBase.h"
#include "Common/CommonInterface.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ABattlefieldCharacterBase::ABattlefieldCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;
	bIsValid = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(50.f, 90.f);

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 400.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	ComponentInit();
	bIsValid = true;
}

// Called when the game starts or when spawned
void ABattlefieldCharacterBase::BeginPlay()
{
	BeginPlayLoad();

	Super::BeginPlay();
	
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

void ABattlefieldCharacterBase::HitActor(AActor* targetActor)
{
	UGameplayStatics::ApplyDamage(targetActor,
		CalDamage(),
		this->GetController(),
		this,
		DamageTypeClass);
	return;
}

USkeletalMeshComponent* ABattlefieldCharacterBase::GetWeaponMesh(int weaponId)
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

	// TODO: 做一个装备的表，先读取衣服，生成新的SkMesh；再读武器，绑到对应插槽上或隐藏。
	/* 1.2 Mesh组件 */
	if (GetMesh()) {
	} else {
		bIsValid = false; // 组件内存申请失败时角色不可用。
		return;
	}
	USkeletalMesh* skMesh = LoadObject<USkeletalMesh>(NULL,
		TEXT("SkeletalMesh'/Game/Animation/SkeletalMesh/SK_Default.SK_Default'"));
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

	/* 1.3 武器管理组件 */
	for (int weaponId = 0; weaponId < 1; weaponId++) {
		USkeletalMeshComponent* skMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(UCommonInterface::CombinFNameId("Weapon", weaponId));
		if (skMeshComp) {
			skMeshComp->SetupAttachment(GetMesh(), TEXT("main_hand"));
		} else {
			bIsValid = false; // 组件内存申请失败时角色不可用。
			return;
		}
		USkeletalMesh* axc = LoadObject<USkeletalMesh>(NULL,
			TEXT("SkeletalMesh'/Game/Character/Mesh/Animation/heavy_weapons/SK_Axc.SK_Axc'"));
		if (axc) {
			FTransform tWeapon = UKismetMathLibrary::MakeTransform(
				FVector(0.f, 0.f, -20.f),
				FRotator(0.f, 0.f, 0.f),
				FVector(1.f, 1.f, 0.8f));
			skMeshComp->SetSkeletalMesh(axc);
			skMeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			skMeshComp->SetRelativeTransform(tWeapon);
		} else {
			UE_LOG(LoadLog, Error, TEXT("Load Weapon Error"));
		}
		Weapon.Emplace(skMeshComp);
	}
}

void ABattlefieldCharacterBase::BeginPlayLoad()
{
	/* step2 : 加载内容 */
	/* 2.1 动画蓝图 */
	UAnimBlueprint* abp = LoadObject<UAnimBlueprint>(NULL,
		TEXT("AnimBlueprint'/Game/Animation/AnimBlueprint/ABP_Default.ABP_Default'"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(abp->GetAnimBlueprintGeneratedClass());

	/* 2.1 虚控件蓝图 */
	CreateStateWidget();
}

float ABattlefieldCharacterBase::CalDamage()
{
	int damage = State->GetStateItem(EnumActorStateItem::EN_ATK);
	int reduce = State->GetStateItem(EnumActorStateItem::EN_DEF);
	return static_cast<float>(damage - reduce);
}

void ABattlefieldCharacterBase::CharacterStateUpdate_Implementation(EnumActorStateItem state)
{
	// 角色状态变更时，通知控件组件更新。
}

void ABattlefieldCharacterBase::CreateStateWidget_Implementation()
{
	// 玩家角色添加视口UI。
	// AI角色添加头顶UI。
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
		break;
	case EnumCharacterOnceAction::EN_JUMP:
		JumpStarted();
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