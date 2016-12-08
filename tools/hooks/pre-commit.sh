#!/bin/bash


BRANCH=`git rev-parse --abbrev-ref HEAD`

if [[ "$BRANCH" == "master" ]]; then
	echo "You are on branch $BRANCH. Are you sure you want to commit to this branch?"
	echo "If so, commit with -n to bypass this pre-commit hook."
	exit 1
fi

# This is old code, which did not work for Git versions later than 1.7 for updating a version file using
# the pre-commit hook
#
# The code was moved to the pre-push hook
#
# This tests whether there are any stashed changes waiting to be added
#git diff --cached --exit-code --quiet
#status1=$?
# This tests whether there are any changes waiting to be added
#git diff --exit-code --quiet
#status2=$?
#if [ "$status1" == "1" ] || ["$status2" == "1"] ; then
#fi
