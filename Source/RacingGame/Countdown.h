// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


/**
 * 
 */
class RACINGGAME_API Countdown 
{
public:
	Countdown();
	~Countdown();

	bool IsCountingDown();
	void SetCountingDown();
	float GetSeconds();
	
	void CountDown(float DeltaTime);

private:
	bool CountingDown;
	float SecondsLeft;

};
