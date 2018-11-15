#!/bin/bash
########################################################################################################################
# 1. Copy into source directory
# 2. Run: sh build_docs.sh
########################################################################################################################

set -ex

rm -fr interop_docs
git clone https://${GITHUB_TOKEN}@git.illumina.com/Bioinformatics/interop.git interop_docs
cd interop_docs

git checkout gh-pages
git reset GH_PAGES_ROOT --hard

mv ../dist/html/* ./

git add .
git commit -am "Added documentation"
git push origin +HEAD
git checkout @{-1}
cd -
rm -fr interop_docs
