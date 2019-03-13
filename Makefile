.PHONY: all build deploy
.ONESHELL:

TERM:=xterm-color

all: build


build:
	@./gradlew build

deploy: build
	@$(make-version-file)
	@./gradlew deploy
	@$(clean-version-file)

define make-version-file
	cat > src/main/deploy/version.txt <<'EOF'
	========================================
	FRC Team 1720 - PhyXTGears

	Software version: $(shell git describe --tags)
	========================================
	EOF
endef

define clean-version-file
	@rm src/main/deploy/version.txt
endef
