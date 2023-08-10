// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveToPosition.generated.h"

/**
 * 
 */
UCLASS()
class STRAWBERRYDASH_API UBTTask_MoveToPosition : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_MoveToPosition();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pawn", meta = (AllowPrivateAccess = true))
		APawn* AIPawn;
};
