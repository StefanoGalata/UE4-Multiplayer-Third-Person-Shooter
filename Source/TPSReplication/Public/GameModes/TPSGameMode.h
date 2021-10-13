// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TPSGameMode.generated.h"

/**
 * The game mode only exists on the Server
 */
UCLASS()
class TPSREPLICATION_API ATPSGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
};
