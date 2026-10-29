#!/bin/bash

# Clear Python environment variables that cause conflicts
unset PYTHONPATH
unset PYTHONHOME

# Run Unreal Editor with the project
/home/yarkowski/UnrealEngine/Editor/Linux_Unreal_Engine_5.4.4/Engine/Binaries/Linux/UnrealEditor "$HOME/Documents/Unreal Projects/MaxAction/MaxAction.uproject" 