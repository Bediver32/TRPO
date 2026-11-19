TEMPLATE = subdirs

SUBDIRS += primelist_lib primelist primelist_gui primelist_tests primelist_gui_tests
primelist.depends = primelist_lib
primelist_gui.depends = primelist_lib
primelist_tests.depends = primelist_lib
primelist_gui_tests.depends = primelist_lib
