TEMPLATE    = subdirs
SUBDIRS     = calculatorform

!static:SUBDIRS += calculatorbuilder \
                   containerextension \
                   customwidgetplugin \
                   taskmenuextension \
                   worldtimeclockbuilder \
                   worldtimeclockplugin

# the sun cc compiler has a problem with the include lines for the form.prf
solaris-cc*:SUBDIRS -= calculatorbuilder \
		       worldtimeclockbuilder
		     

# install
sources.files = README *.pro
sources.path = $$[QT_INSTALL_EXAMPLES]/qttools/designer
INSTALLS += sources

symbian: CONFIG += qt_example
maemo5: CONFIG += qt_example
