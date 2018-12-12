BUILD_DIRS=build.*

all: release

system:
	./scripts/image

release:
	./scripts/image release

image:
	./scripts/image mkimage

noobs:
	./scripts/image noobs

amlpkg:
	./scripts/image amlpkg

system_mt:
	./scripts/image_mt

release_mt:
	./scripts/image_mt release

image_mt:
	./scripts/image_mt mkimage

noobs_mt:
	./scripts/image_mt noobs

amlpkg_mt:
	./scripts/image_mt amlpkg

addons_mt:
	./scripts/create_addon_mt all

clean:
	rm -rf $(BUILD_DIRS)/* $(BUILD_DIRS)/.stamps

distclean:
	rm -rf ./.ccache ./$(BUILD_DIRS)

src-pkg:
	tar cvJf sources.tar.xz sources .stamps
