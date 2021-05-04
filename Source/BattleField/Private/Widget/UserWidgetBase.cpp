// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/UserWidgetBase.h"
#include "Character/BattlefieldCharacterBase.h"
#include "Components/ProgressBar.h"

void UUserWidgetBase::StateUpdate(ABattlefieldCharacterBase* character, EnumActorStateItem stateItem)
{
	UActorState* state = character->GetState();
	if (!state || !HP) {
		return;
	}
	switch (stateItem) {
	case EnumActorStateItem::EN_HP:
		HP->SetPercent(state->GetHpPercent());
		break;
	case EnumActorStateItem::EN_MP:
		break;
	case EnumActorStateItem::EN_ATK:
		break;
	case EnumActorStateItem::EN_DEF:
		break;
	case EnumActorStateItem::EN_SPD:
		break;
	}
}