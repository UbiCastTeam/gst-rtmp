# so,
# - don't put it in SUBDIRS so automake doesn't descend
# - put it in DIST_SUBDIRS so make dist picks up the subtree
# - add an all-local hook so it does get built
# this also satisfies make distcheck

SUBDIRS = 
DIST_SUBDIRS = librtmp
TMP_DIST_DIR=librtmp-dist
DIST_DIR=$(TMP_DIST_DIR)/.librtmp
EXTRA_DIST=Makefile.am
MAINTAINERCLEANDIRS = $(DIST_SUBDIRS)

all-local:
	cd librtmp && $(MAKE)

clean-local:
	cd librtmp && $(MAKE) clean

dist-clean:
	cd librtmp && $(MAKE) distclean
	rm -rf $(TMP_DIST_DIR)
	rm -f Makefile
	rm -f librtmp/.version
	rm -f librtmp/.config

distclean: dist-clean

maintainer-clean: distclean
	rm -rf $(MAINTAINERCLEANDIRS)

maintainerclean: maintainer-clean

dist-local:
	GIT_DIR=librtmp/.git git checkout-index --prefix=$(TMP_DIST_DIR)/librtmp/ -a
	touch $(TMP_DIST_DIR)/librtmp/config.mak
	echo "Patching librtmp ./configure"
	sed -e '/Unknown option/ {N;N;s/exit 1//; }' $(TMP_DIST_DIR)/librtmp/configure > $(TMP_DIST_DIR)/librtmp/configure.tmp
	mv $(TMP_DIST_DIR)/librtmp/configure.tmp $(TMP_DIST_DIR)/librtmp/configure
	chmod +x $(TMP_DIST_DIR)/librtmp/configure

distdir: dist-local
	cp -r $(TMP_DIST_DIR)/librtmp ${distdir}
	cp -f $(top_srcdir)/gst-libs/ext/Makefile.am $(top_srcdir)/gst-libs/ext/Makefile.in ${distdir}
	rm -rf $(TMP_DIST_DIR)

dist: dist-local
	cd $(TMP_DIST_DIR) && tar -czf librtmp.tar.gz librtmp
	mv $(TMP_DIST_DIR)/librtmp.tar.gz ./
	rm -rf $(TMP_DIST_DIR)
