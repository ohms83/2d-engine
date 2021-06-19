#! /usr/bin/python3
import sys
import argparse
import shutil
import os

parser = argparse.ArgumentParser(description="Set up a new project.")
parser.add_argument("--name", "-n", required=True, help="Project name")
parser.add_argument("--bundle", "-b", required=False, help="Android, iOS and Mac app bundle ID. This is optional if you don't deploy to those platforms.")
parser.add_argument("--directory", "-d", required=False, default=".", help="Target output directory. The default value is current directory.")

arg = parser.parse_args()
scriptRoot = sys.path[0]

fileLists = [
    "CMakeLists.txt",
    "Resources/config/game.ini",
    "proj.android/app/AndroidManifest.xml",
    "proj.android/app/build.gradle",
    "proj.ios_mac/ios/Info.plist",
    "proj.ios_mac/mac/Info.plist",
    "proj.android/app/res/values/strings.xml",
]

if not os.path.exists(arg.directory):
    os.makedirs(arg.directory)
    pass

templateFolder = os.path.join(scriptRoot, "template")

# Copy files to destination folder
print(f"Copying files to {arg.directory}")
for root, dirs, files in os.walk(templateFolder):
    for _file in files:
        path = os.path.join(root, _file)
        shutil.copy2(path, arg.directory)
        pass
    for _dir in dirs:
        path = os.path.join(root, _dir)
        shutil.copytree(path, os.path.join(arg.directory, _dir))
        pass
    break

appNameTag = "<$appname>"
bundleNameTag = "<$bundlename>"
for path in fileLists:
    # Replace all the current path separators with OS specific ones. This is to support Windows's stupid path separator (\)
    path = path.replace("/", os.path.sep)
    path = os.path.join(arg.directory, path)
    fileData = ""

    with open(path, "r") as inFile:
        fileData = inFile.read()
        index = fileData.find(appNameTag)
        if index >= 0:
            print(f"{path}: Replaced {appNameTag} with {arg.name}")
            fileData = fileData.replace(appNameTag, arg.name)

        index = fileData.find(bundleNameTag)
        if arg.bundle != None and index >= 0:
            print(f"{path}: Replaced {bundleNameTag} with {arg.bundle}")
            fileData.replace(bundleNameTag, arg.bundle)
            pass
        pass
    with open(path, "w") as outFile:
        outFile.write(fileData)
        pass

    pass