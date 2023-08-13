// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_CheckPlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class STRAWBERRYDASH_API UBTTask_CheckPlayerDistance : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_CheckPlayerDistance();
	AAIController* AIController;
	const APawn* AIPawn;
	AActor* player;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
		float SearchRadius{ 1000.f };
	
};
