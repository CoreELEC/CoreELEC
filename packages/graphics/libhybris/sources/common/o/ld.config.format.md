# Linker config file format

This document describes format of /system/etc/ld.config.txt file. This file can be used to customize
linker-namespace setup for dynamic executables.

## Overview

The configuration consists of 2 parts
1. Mappings - maps executable locations to sections
2. Sections - contains linker-namespace configuration

## Mappings

This part of the document maps location of an executable to a section. Here is an example

The format is `dir.<section_name>=<directory>`

The mappings should be defined between start of ld.config.txt and the first section.

## Section

Every section starts with `[section_name]` (which is used in mappings) and it defines namespaces
configuration using set of properties described in example below.

## Example

```
# The following line maps section to a dir. Binraies ran from this location will use namespaces
# configuration specified in [example_section] below
dir.example_section=/system/bin/example

# Section starts
[example_section]

# When this flag is set to true linker will set target_sdk_version for this binary to
# the version specified in <dirname>/.version file, where <dirname> = dirname(executable_path)
#
# default value is false
enable.target.sdk.version = true

# This property can be used to declare additional namespaces.Note that there is always the default
# namespace. The default namespace is the namespace for the main executable. This list is
# comma-separated.
additional.namespaces = ns1

# Each namespace property starts with "namespace.<namespace-name>" The following is configuration
# for the default namespace

# Is namespace isolated - the default value is false
namespace.default.isolated = true

# Default namespace search path. Note that ${LIB} here is substituted with "lib" for 32bit targets
# and with "lib64" for 64bit ones.
namespace.default.search.paths = /system/${LIB}:/system/other/${LIB}

# ... same for asan
namespace.default.asan.search.paths = /data/${LIB}:/data/other/${LIB}

# Permitted path
namespace.default.permitted.paths = /system/${LIB}

# ... asan
namespace.default.asan.permitted.paths = /data/${LIB}

# This declares linked namespaces - comma separated list.
namespace.default.links = ns1

# For every link define list of shared libraries. This is list of the libraries accessilbe from
# default namespace but loaded in the linked namespace.
namespace.default.link.ns1.shared_libs = libexternal.so:libother.so

# This part defines config for ns1
namespace.ns1.isolated = true
namespace.ns1.search.paths = /vendor/${LIB}
namespace.ns1.asan.search.paths = /data/vendor/${LIB}
namespace.ns1.permitted.paths = /vendor/${LIB}
namespace.ns1.asan.permitted.paths = /data/vendor/${LIB}

# and links it to default namespace
namespace.ns.links = default
namespace.ns.link.default.shared_libs = libc.so:libdl.so:libm.so:libstdc++.so
```

