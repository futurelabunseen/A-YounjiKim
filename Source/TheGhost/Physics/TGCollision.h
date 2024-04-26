
#pragma once

#include "CoreMinimal.h" // 기본적인 헤더

#define CPROFILE_TGCAPSULE TEXT("TGCapsule") // Character Capsule Profile (Preset) 을 앞으로 CPROFIILE_TGCAPSULE로 지칭
#define CPROFILE_TGTRIGGER TEXT("TGTrigger") // Trigger Profile (Preset)
#define CCHANNEL_ABACTION ECC_GameTraceChannel1 // Trace Channel
// 위 셋은 ini 파일에서 지정된 채널 열거형 값으로 변환된다.