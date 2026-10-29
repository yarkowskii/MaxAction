// TestBuild.h - Simple file to test the build system
#pragma once

#include "CoreMinimal.h"

/**
 * Simple test class to verify build system
 */
class MAXACTION_API FTestBuild
{
public:
    FTestBuild() = default;
    ~FTestBuild() = default;
    
    int32 GetTestValue() const { return 42; }
}; 