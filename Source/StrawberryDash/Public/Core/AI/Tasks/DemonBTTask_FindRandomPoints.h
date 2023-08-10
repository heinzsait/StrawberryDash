// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "DemonBTTask_FindRandomPoints.generated.h"

/**
 * 
 */
UCLASS()
class STRAWBERRYDASH_API UDemonBTTask_FindRandomPoints : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UDemonBTTask_FindRandomPoints();
	AAIController* AIController;
	const APawn* AIPawn;
	FVector Origin;;

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = true))
		float SearchRadius{ 500.f };
};
