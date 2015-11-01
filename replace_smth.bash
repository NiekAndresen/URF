#!/bin/bash
arr[0]="URFCycle"
for OLD_FUNCTION_NAME in "${arr[@]}"
do
    echo "old: $OLD_FUNCTION_NAME"
    NEW_FUNCTION_NAME="URF_cycle"
    echo "new: $NEW_FUNCTION_NAME"
    SED_ARG_STR="s/$OLD_FUNCTION_NAME/$NEW_FUNCTION_NAME/g"
    grep -rl $OLD_FUNCTION_NAME URFcode/ | xargs sed -i $SED_ARG_STR
done
