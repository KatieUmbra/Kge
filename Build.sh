#!/bin/sh

GREEN="\033[1;32m"
RED="\033[0;31m"
GRAY="\033[38;5;240m"
RESET="\033[0m"
cmake328=~/.local/cmake-3.28.0-rc2/bin/cmake

log() {
	echo -e "${GREEN}[INFO]${RESET}: $1"
}

error() {
	echo -e "${RED}[ERROR]${RESET}: $1"
}

build() {
	log "Building cmake"
	echo -e "$GRAY"
	$cmake328 -B build -GNinja
	echo -e "$RESET"
	if [ ! $? ];
	then
		error "Failed to build cmake"
	fi
	log "Building files"
	echo -e "$GRAY"
	$cmake328 --build build
	echo -e "$RESET"
	if [ ! $? ];
	then
		error "Failed build files"
	fi
}

clean() {
	log "Cleaning build files"
	rm -rf build
	if [ ! $? ];
	then
		error "Failed to clean files"
	fi
}

run() {
	log "Executing task: clean"
	clean
	log "Executing task: build"
	build
	log "Running binary"
	./build/oglgame
	if [ ! $? ]; then
		error "Failed to run binary"
	fi
}

if [ "$1" == "--run" ] || [ "$1" == "-r" ]; then
	run
elif [ "$1" == "--build" ] || [ "$1" == "-b" ]; then
	build
elif [ "$1" == "--clean" ] || [ "$1" == "-c" ]; then
	clean
elif [ "$1" == "--help" ] || [ "$1" == "-h" ]; then
	log "Options are [--run | -r] [--build | -b] [--clean | -c] [--help | -h]"
else
	error "Invalid usage. Options are [--run | -r] [--build | -b] [--clean | -c] [--help | -h]"
fi
