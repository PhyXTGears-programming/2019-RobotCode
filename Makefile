.PHONY: all build deploy
.ONESHELL:

TERM:=xterm-color

all: build


build:
	@./gradlew build

deploy: build
	@./gradlew deploy
