#!/bin/sh

# Bail out on PR builds immediately
if [[ ${TRAVIS_PULL_REQUEST} != "false" ]]; then
    echo "Skipping deployment for PR build"
    exit 0
fi

# Assumed to be run either on a development branch or a tagged master branch
if [[ ${TRAVIS_BRANCH} == "develop" ]]; then
    APPNAME="helio-dev"
elif [[ ${TRAVIS_TAG} != "" ]]; then
    APPNAME="helio-${TRAVIS_TAG}"
else
    echo "Skipping deployment: will only run either on a tagged commit, or a develop branch commit"
    exit 0
fi

cd ${TRAVIS_BUILD_DIR}/Projects/Deployment

echo "Exporting"
xcrun xcodebuild -exportArchive \
  -archivePath /tmp/archive.xcarchive \
  -exportPath /tmp \
  -exportOptionsPlist ./Travis/export-options-ios.plist;

echo "Uploading to iTunesConnect"
ln -s "/Applications/Xcode.app/Contents/Applications/Application Loader.app/Contents/Frameworks/ITunesSoftwareService.framework/Support/altool" /usr/local/bin/altool
ln -s "/Applications/Xcode.app/Contents/Applications/Application Loader.app/Contents/itms" /usr/local/bin/itms # itms is needed, otherwise altool will not work correctly
altool --upload-app -f "/tmp/Helio - App.ipa" -u ${IOS_ITC_USERNAME} -p ${IOS_ITC_APP_PASSWORD}
