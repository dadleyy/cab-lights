ACC=arduino-cli

BOARD=adafruit:samd:adafruit_trinket_m0
DEBUG_BIN=target/debug/cab-lights.ino.bin
RELEASE_BIN=target/release/cab-lights.ino.bin
RELEASE_FLAGS=--build-property "compiler.cpp.extra_flags=\"-DCAB_LIGHTS_RELEASE=1\""

.PHONY: all release clean upload-release

all: $(DEBUG_BIN)

release: $(RELEASE_BIN)

clean:
	@rm -rf target

$(DEBUG_BIN): cab-lights.ino
	$(ACC) compile -v -b $(BOARD) --output-dir ./target/debug

$(RELEASE_BIN): cab-lights.ino
	$(ACC) compile -v -b $(BOARD) $(RELEASE_FLAGS)  --output-dir ./target/release

upload-release: $(RELEASE_BIN)
	$(ACC) upload -v -b $(BOARD) --input-dir ./target/release -p `./find-board.sh`
