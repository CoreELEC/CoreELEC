# LibreELEC Build System — Agent Operational Guide

## ⚠️ Critical Guardrails
1. **Never run full image builds (`make`, `make image`, `make release`, `./scripts/image`) autonomously.** They take hours and dozens of gigabytes. Always test changes by building only that specific package (`./scripts/build_mt <pkg>`). Full image builds (`PROJECT=... ARCH=... make image`) are strictly opt-in when requested by the user.
2. **Never build as root.** `EUID -eq 0` terminates the build.
3. **No spaces in workspace paths.** Build scripts abort if `${PWD}` contains spaces.
4. **No autonomous issues, PRs or pushes.** Never create, submit, or comment on GitHub issues/PRs, and never `git push` (least of all `--force`), unless explicitly asked by the user. Keep work local.
5. **Follow [STANDARDS.md](STANDARDS.md)** for all shell, `package.mk` and device-side scripts. Device scripts run under busybox `sh`, not bash.

---

## Target Configuration
Configure builds via environment variables passed to commands (defaults: `PROJECT=Generic`, `ARCH=x86_64`, `DISTRO=LibreELEC`):
- **Generic PC (x86_64):** `PROJECT=Generic ARCH=x86_64`
- **Raspberry Pi 5 (64-bit):** `PROJECT=RPi DEVICE=RPi5 ARCH=aarch64`
- Variables: `PROJECT` (hardware family), `DEVICE` (device sub-flavor), `ARCH` (`x86_64`, `arm`, `aarch64`), `DISTRO`.

---

## Build & Package Commands

> **Host dependencies:** `build_mt`, `create_addon` and `scripts/image` run `./scripts/checkdeps` themselves. Run it by hand only to diagnose missing host tools.
>
> **Build logs:** `build_mt` writes one log per package to `${BUILD}/.threads/logs/`. Read the failing package's log there rather than re-running the build serially.

| Task | Command |
| :--- | :--- |
| **Unpack & apply patches** | `PROJECT=... ARCH=... ./scripts/unpack <pkg>` |
| **Parallel build (recommended)** | `PROJECT=... ARCH=... ./scripts/build_mt <pkg>[:target\|host]` |
| **Sequential build (debug only)** | `PROJECT=... ARCH=... ./scripts/build <pkg>[:target\|host]` |
| **Clean package & stamps** | `PROJECT=... ARCH=... ./scripts/clean <pkg>` |
| **Validate package metadata** | `./tools/pkgcheck <pkg>` |
| **Build addon package** | `PROJECT=... ARCH=... ./scripts/create_addon <addon_pkg>` |
| **Inspect config / plan** | `./tools/viewconfig` / `./tools/viewplan <pkg>` |
| **Full image build (authorized only)** | `PROJECT=... ARCH=... make image` |

*Target qualifiers:* `:target` (default), `:host` (host build tools), `:bootstrap`, `:init`.

### Addon Packaging Notes
- **Output:** `target/addons/<ADDON_VERSION>/<DEVICE>/<ARCH>/<addon_id>/<addon_id>-<version>.zip`  
  *Example:* `target/addons/12.80.7/RPi5/aarch64/tools.ffmpeg-tools/tools.ffmpeg-tools-12.80.7.6.zip`
- **Never overwritten automatically:** If the `.zip` archive exists, `scripts/install_addon` warns and keeps the old one. To rebuild locally, pass `ADDON_OVERWRITE=yes` or delete the existing `.zip`. Do not bump `PKG_REV` just to rebuild; bump it only as part of a real addon change (Kodi needs it to detect the update).

---

## Package System
Packages live under `packages/<category>/<name>/package.mk`.

| Variable | Purpose |
| :--- | :--- |
| `PKG_NAME`, `PKG_VERSION`, `PKG_SHA256` | Package identity and download checksum |
| `PKG_URL` | Source tarball or git repository URL |
| `PKG_DEPENDS_TARGET` / `PKG_DEPENDS_HOST` | Dependencies for target or host toolchain |
| `PKG_TOOLCHAIN` | Build system: `cmake`, `meson`, `autotools`, `make`, `manual` |
| `PKG_CMAKE_OPTS_TARGET` / `PKG_MESON_OPTS_TARGET` | Target configure/build flags |
| `PKG_BUILD_FLAGS` | LE build flags (e.g. `+pic`, `-gold`, `-sysroot`, `-parallel`) |
| `PKG_ARCH` | Architecture restrictions (`any`, `x86_64`, `aarch64`, `arm`) |

- **Hooks:** Named functions execute at build stages (e.g. `pre_configure_target()`, `post_makeinstall_target()`).
- **SPDX License:** `PKG_LICENSE` must use valid SPDX expressions (e.g. `GPL-2.0-only`, `MIT`) matching text files in `licenses/<identifier>.txt`.

---

## Updating Packages & Patches
- **Automated version update:** `./tools/update-pkg <pkg> <new_version>` (updates `PKG_VERSION` and `PKG_SHA256`). For addons, also bump `PKG_REV`.
- **Manual version update:** Bump `PKG_VERSION` in `packages/.../<pkg>/package.mk` -> fetch with `PROJECT=... ARCH=... ./scripts/get <pkg>` -> update `PKG_SHA256` -> validate with `./tools/pkgcheck <pkg>`.
- **Refresh patches (quilt):** `PROJECT=... ARCH=... ./tools/refresh-patches <pkg>` (requires `quilt:host` via `scripts/build_mt quilt:host`).
- **Manual patch workflow:**
  1. Unpack: `PROJECT=... ARCH=... ./scripts/unpack <pkg>`
  2. In `build.<DISTRO>-<DEVICE or PROJECT>.<ARCH>-<VERSION>/build/<pkg>-<version>/`, run `git init` and commit the pristine tree before editing, so the patch is generated from source rather than hand-written
  3. Modify the source, commit, and save `git format-patch -1` output as `####-subject.patch` in `packages/.../<pkg>/patches/`: 4 digits from `0001`, no package name prefix
  4. Test clean application: `PROJECT=... ARCH=... ./scripts/clean <pkg> && PROJECT=... ARCH=... ./scripts/unpack <pkg>`

---

## Repository & Build Layout
Where to find files and artifacts across the tree:

| Location | Contents |
| :--- | :--- |
| `packages/` | Core system packages (`linux`, `network`, `sysutils`, `mediacenter`, etc.) |
| `packages/addons/` | Official LibreELEC addons (`service`, `virtual`, etc.) |
| `packages/mediacenter/kodi-binary-addons/` | Kodi binary addons (PVR, audio encoders, inputstream, etc.) |
| `projects/<PROJECT>/` | Hardware platform & device options, kernel defconfigs, and board patches |
| `scripts/` | Build system engine (`build`, `build_mt`, `clean`, `unpack`, `create_addon`, `image`) |
| `tools/` | Developer CLI tools (`update-pkg`, `refresh-patches`, `pkgcheck`, `viewconfig`, `viewplan`) |
| `config/` | Global build options, architecture defaults (`arch.*`), and utility functions |
| `distributions/` | Distro identity, version strings, and distro-specific options |
| `licenses/` | Plain-text SPDX license files matching `PKG_LICENSE` |
| `sources/` | Download cache for upstream tarballs and Git repositories |
| `target/` | Final distribution images (`.img.gz`) and addon zip packages (`target/addons/`) |
| `build.<DISTRO>-<DEVICE or PROJECT>.<ARCH>-<VER>/` | Working build directory: `.stamps/` (completion markers), `build/<pkg>-<ver>/` (sources and logs like `CMakeCache.txt`, `meson-logs/`), and `toolchain/` |

*Stamps note:* If `.stamps/<pkg>/build_<target>` exists and the hash matches, the build system skips compilation ("already built"). Run `./scripts/clean <pkg>` to force rebuild.

---

## Key Files
Core build system files:

| Path | Purpose |
| :--- | :--- |
| `distributions/LibreELEC/version` | Version strings (`DISTRO_VERSION`, `OS_VERSION`, `ADDON_VERSION`) |
| `config/options` | Global build environment defaults and checks |
| `config/functions` | Core bash utility functions sourced by build scripts and package hooks |
| `scripts/genbuildplan.py` | Package dependency resolver and build plan generator |

---

## Agent Pitfalls & Gotchas
- **Source Edits in `build/` are Ephemeral:** Never leave source code changes only in `build.<...>/build/<pkg>-<ver>/`. The build folder is wiped on clean. Always save permanent changes as patches in `packages/.../<pkg>/patches/`.
- **Target Sysroot vs. Host OS:** Compilation errors (`#include <foo.h> not found`) mean target dependencies are missing (`PKG_DEPENDS_TARGET="foo"` in `package.mk`). Never fix a target build error by installing a host `-dev` package; host libraries are only for `:host` builds.
- **Project & Device Overlays:** Board-specific kernel defconfigs, drivers, and patches live under `projects/<PROJECT>/devices/<DEVICE>/` and override base packages. Check device trees and device `options` when troubleshooting hardware-specific builds.
- **Git Revisions:** Always use full 40-character commit hashes for `PKG_VERSION` in git-based packages (`STANDARDS.md`), never abbreviated 7-character hashes.

---

## Contribution & Code Quality Standards
Adapted from upstream Kodi best practices:
- **Minimal Scope:** Make the smallest change that solves the issue. No speculative hardening, refactoring, or formatting churn. If scope must expand, stop and report why first.
- **Comments:** Be concise; explain *why*, not obvious *what*. Never include rejected attempts or conversational history. Never remove or alter existing human comments.
- **Commits:** Subject is `<pkg>: <what changed>`, e.g. `<pkg>: update to x.y.z` or `<pkg>: update addon (N)`. Explain *why* in the body, wrap body text at 76 columns. Squash intermediate commits.
- **New files:** A new `package.mk` starts with `# SPDX-License-Identifier: GPL-2.0-only`. This is the licence of the recipe, separate from the upstream `PKG_LICENSE`.
- **Pre-submission:** Clean diff, no unintended modifications or AI residue, package builds via `PROJECT=... ARCH=... ./scripts/build_mt <pkg>`, and metadata passes `./tools/pkgcheck <pkg>`.
