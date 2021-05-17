#!/bin/bash

# Get to the root folder
pathScript=$(dirname "$0")
cd $pathScript/..

# Retrieve the version of the program
varNameVersion="PROJECT_VERSION"
version=$(cat src/src.pro | grep "^$varNameVersion" | tr -d "\" =" | sed "s/$varNameVersion//g")

# Create a list of files to archive
listArchive=$(ls | grep -v "archive" | grep -v "bin")

# Create an archive
dirArchive="archive"
tempNameArchive="tempArchive.zip"
echo $listArchive | xargs zip -r "$dirArchive/$tempNameArchive" 

# Set the archive name
cd $dirArchive
nameArchive="$(date +"%Y.%m.%d") - v$version.zip"
mv $tempNameArchive "$nameArchive"