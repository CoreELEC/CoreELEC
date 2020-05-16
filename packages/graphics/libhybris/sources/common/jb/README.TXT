Android Dynamic Linker Design Notes
===================================

Introduction:
-------------

This document provides several notes related to the design of the Android
dynamic linker.


Prelinking:
-----------

System libraries in Android are internally prelinked, which means that
any internal relocations within them are stripped from the corresponding
shared object, in order to reduce size and speed up loading.

Such libraries can only be loaded at the very specific virtual memory address
they have been prelinked to (during the build process). The list of prelinked
system libraries and their corresponding virtual memory address is found in
the file:

   build/core/prelink-linux-<arch>.map

It should be updated each time a new system library is added to the
system.

The prelink step happens at build time, and uses the 'soslim' and 'apriori'
tools:

  - 'apriori' is the real prelink tool which removes relocations from the
    shared object, however, it must be given a list of symbols to remove
    from the file.

  - 'soslim' is used to find symbols in an executable ELF file
    and generate a list that can be passed to 'apriori'.

By default, these tools are only used to remove internal symbols from
libraries, though they have been designed to allow more aggressive
optimizations (e.g. 'global' prelinking and symbol stripping, which
prevent replacing individual system libraries though).

You can disable prelinking at build time by modifying your Android.mk with
a line like:

    LOCAL_PRELINK_MODULE := false


Initialization and Termination functions:
-----------------------------------------

The Unix Sys V Binary Interface standard states that an
executable can have the following entries in its .dynamic
section:

  DT_INIT
      Points to the address of an initialization function
      that must be called when the file is loaded.

  DT_INIT_ARRAY
      Points to an array of function addresses that must be
      called, in-order, to perform initialization. Some of
      the entries in the array can be 0 or -1, and should
      be ignored.

      Note: this is generally stored in a .init_array section

  DT_INIT_ARRAYSZ
      The size of the DT_INITARRAY, if any

  DT_FINI
      Points to the address of a finalization function which
      must be called when the file is unloaded or the process
      terminated.

  DT_FINI_ARRAY
      Same as DT_INITARRAY but for finalizers. Note that the
      functions must be called in reverse-order though

      Note: this is generally stored in a .fini_array section

  DT_FINI_ARRAYSZ
      Size of FT_FINIARRAY

  DT_PREINIT_ARRAY
      An array similar to DT_INIT_ARRAY which must *only* be
      present in executables, not shared libraries, which contains
      a list of functions that need to be called before any other
      initialization function (i.e. DT_INIT and/or DT_INIT_ARRAY)
      in the executable or any of its libraries.

      Note: this is generally stored in a .preinit_array section

  DT_PREINIT_ARRAYSZ
      The size of DT_PREINIT_ARRAY

If both a DT_INIT and DT_INITARRAY entry are present, the DT_INIT
function must be called before the DT_INITARRAY functions.

Consequently, the DT_FINIARRAY must be parsed in reverse order before
the DT_FINI function, if both are available.

Note that the implementation of static C++ constructors is very
much processor dependent, and may use different ELF sections.

On the ARM (see "C++ ABI for ARM" document), the static constructors
must be called explicitly from the DT_INIT_ARRAY, and each one of them
shall register a destructor by calling the special __eabi_atexit()
function (provided by the C library). The DT_FINI_ARRAY is not used
by static C++ destructors.

On x86, the lists of constructors and destructors are placed in special
sections named ".ctors" and ".dtors", and the DT_INIT / DT_FINI functions
are in charge of calling them explicitly.


Debugging:
----------

It is possible to enable debug output in the dynamic linker. To do so,
follow these steps:

1/ Modify the line in Android.mk that says:

    LOCAL_CFLAGS += -DLINKER_DEBUG=0

  Into the following:

    LOCAL_CFLAGS += -DLINKER_DEBUG=1

2/ Force-rebuild the dynamic linker:

    cd bionic/linker
    mm -B

3/ Rebuild a new system image.

You can increase the verbosity of debug traces by defining the DEBUG
environment variable to a numeric value from 0 to 2. This will only
affect new processes being launched.

By default, traces are sent to logcat, with the "linker" tag. You can
change this to go to stdout instead by setting the definition of
LINKER_DEBUG_TO_LOG to 0 in "linker_debug.h".
