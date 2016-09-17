#!/bin/bash


BRANCH=`git rev-parse --abbrev-ref HEAD`

if [[ "$BRANCH" == "master" ]]; then
	echo "You are on branch $BRANCH. Are you sure you want to commit to this branch?"
	echo "If so, commit with -n to bypass this pre-commit hook."
	exit 1
fi

git diff --cached --exit-code --quiet
status1=$?
git diff --exit-code --quiet
status2=$?
if [ "$status1" == "1" ] || ["$status2" == "1"] ; then
version=`git describe --tags --dirty=-src`
cp CMakeLists.txt .CMakeLists.txt.bak
sed "s/set(ARCHIVE_VERSION .*)/set(ARCHIVE_VERSION \"${version}\")/g" .CMakeLists.txt.bak > CMakeLists.txt
git add CMakeLists.txt
git commit --amend -C HEAD --no-verify
fi
