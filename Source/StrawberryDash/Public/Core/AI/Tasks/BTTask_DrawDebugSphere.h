// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_DrawDebugSphere.generated.h"

/**
 * 
 */
UCLASS()
class STRAWBERRYDASH_API UBTTask_DrawDebugSphere : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_DrawDebugSphere();

private:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radius", meta = (AllowPrivateAccess = true))
		float SphereRadius{ 50.f };
};
