## Project Coding Standards

### Build system scripts and config:

* Executable build scripts use `#!/bin/bash`. Sourced config fragments (`config/functions`, `config/options`, `config/arch.*` etc.) have no shebang as they are not executed directly
* Avoid backtick command substitution (subshells). Use `$()` instead
* String comparison: use `=` not `==`
* Numeric comparison: use `-eq` not `=` etc.
* For simple tests, `[ expr1 -o expr2 ]`/`[ expr1 -a expr2 ]` is preferred over the extended form `[[ expr1 || expr2 ]]`/`[[ expr1 && expr2 ]]`. Use `[[ ]]` when regex (`=~`) or glob/pattern matching is needed — these require `[[ ]]`. When short-circuit evaluation is required but `[[ ]]` should be avoided, use command grouping: `[ test1 ] && { [ test2 ] && [ test3 ]; }`. See [test constructs](https://www.tldp.org/LDP/abs/html/testconstructs.html)
* Shell variables should use braces ie. use `${FOO}` not `$FOO`
* Use double-quotes (") around variables (or sequences that include variables) when possible to avoid issues with special characters, eg. `cd "$PKG_DIR/scripts"`, although paths should no longer include spaces after #3351. See [quoting variables](https://www.tldp.org/LDP/abs/html/quotingvar.html)
* Use `. config/blah` to source a file, don't use `source config/blah`
* To be efficient, avoid forking child processes (`sed`, `cut`, etc.) when a shell built-in can be used instead
* Avoid long lines (> 90 columns) unless it aids maintainability/processing (eg. `LINUX_DEPENDS`, `PKG_DEPENDS_TARGET` etc.)
* Use `set -e` or `set -euo pipefail` in scripts that should abort on error
* Use [ShellCheck](https://www.shellcheck.net/) to check scripts for common mistakes

### Scripts that run on the device:

* Use `#!/bin/sh` — the runtime shell is busybox sh/ash; bash is not available
* Write to the current POSIX standard; bash extensions cannot be used:
  * No `[[ ]]` extended tests — use `[ ]`
  * No arrays, no `source`, no `=~` regex, no process substitution
  * No `$'...'` escape syntax
* Use [ShellCheck](https://www.shellcheck.net/) with `--shell=sh` to check scripts for common mistakes

### Within `package.mk`:

* See [packages/README.md](packages/README.md) for more detailed package structure information
* When using git revisions, the full 40-char revision is to be used
* Prefix package specific variables with `PKG_` as they are automatically unset before `package.mk` is sourced
* Use `PKG_VAR+=" value"` to append to a variable, not `PKG_VAR="${PKG_VAR} value"`
* When creating a directory, related lines are indented:
```
  cd ${INSTALL}/blah
    cp -P foo ${INSTALL}/blah
```
* The use of `foo && bar` is allowed, but care should be taken so that package functions do not unintentionally exit with a non-zero exit code in which case `foo && bar || true` may be required, or alternatively use the multi-line `if foo; then bar; fi` pattern in place of `foo && bar`
