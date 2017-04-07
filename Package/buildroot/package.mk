################################################################################
#
# iopcvm
#
################################################################################

IOPCVM_VERSION       = <BUILDROOT_VERSION>
IOPCVM_SITE          = $(call github,YuanYuLin,iopcvm,$(IOPCVM_VERSION))
#IOPCVM_SITE          = file:///tmp
#IOPCVM_SOURCE        = iopcvm.tar.bz2
IOPCVM_LICENSE       = GPLv2+
IOPCVM_LICENSE_FILES = COPYING

IOPCVM_DEPENDENCIES  = host-pkgconf libiopccommon
IOPCVM_PACKAGE_DIR  = $(BASE_DIR)/packages/iopcvm

IOPCVM_EXTRA_CFLAGS =                                        \
	-DTARGET_LINUX -DTARGET_POSIX                           \


IOPCVM_MAKE_ENV =                          \
	CROSS_COMPILE=$(TARGET_CROSS)       \
	BUILDROOT=$(TOP_DIR)                \
	SDKSTAGE=$(STAGING_DIR)             \
	TARGETFS=$(IOPCVM_PACKAGE_DIR)     \
	TOOLCHAIN=$(HOST_DIR)/usr           \
	HOST=$(GNU_TARGET_NAME)             \
	SYSROOT=$(STAGING_DIR)              \
	JOBS=$(PARALLEL_JOBS)               \
	$(TARGET_CONFIGURE_OPTS)            \
	CFLAGS="$(TARGET_CFLAGS) $(IOPCVM_EXTRA_CFLAGS)"

define IOPCVM_BUILD_CMDS
	$(IOPCVM_MAKE_ENV) $(MAKE) -C $(@D)
endef

define IOPCVM_INSTALL_TARGET_DIR
	mkdir -p $(TARGET_DIR)/usr/local/bin/
	cp -avr $(IOPCVM_PACKAGE_DIR)/usr/local/bin/iopcvm $(TARGET_DIR)/usr/local/bin/iopcvm
endef

define IOPCVM_INSTALL_TARGET_CMDS
	mkdir -p $(IOPCVM_PACKAGE_DIR)/usr/local/bin/
	$(INSTALL) -m 0755 -D $(@D)/iopcvm.elf	$(IOPCVM_PACKAGE_DIR)/usr/local/bin/iopcvm
	$(IOPCVM_INSTALL_TARGET_DIR)
endef

$(eval $(generic-package))
