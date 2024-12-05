#! /bin/bash

Z3_REPO="https://github.com/microsoft/vcpkg.git"

REQUIRED_PACKAGES="git curl zip unzip tar g++ python3 pkg-config"
# checking if all required packages are installed
for package in $REQUIRED_PACKAGES; do
    if ! dpkg -l | grep -q $package; then
        echo "Installing $package"
        sudo apt install -y $package
    fi
done

# installing vcpkg
git clone $Z3_REPO
cd vcpkg
./bootstrap-vcpkg.sh
./vcpkg install z3