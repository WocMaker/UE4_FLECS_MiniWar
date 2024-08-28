// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "DemoWorldSettings.generated.h"

/**
 * 
 */
UCLASS()
class UE4MINECS_API ADemoWorldSettings : public AWorldSettings
{
	GENERATED_BODY()
public:
	ADemoWorldSettings();
	bool bisUseECS;
};

