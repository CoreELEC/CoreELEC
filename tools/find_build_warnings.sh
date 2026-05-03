#!/bin/bash

# SPDX-License-Identifier: GPL-2.0-only
# Copyright (C) 2026-present Team LibreELEC (https://libreelec.tv)

# find_build_warnings.sh - Report compiler warnings and errors from LibreELEC package build logs
# Run from LibreELEC root: tools/find_build_warnings.sh [logdir] [all|errors|warnings]

MODE="${2:-all}"   # all | errors | warnings

# Ignore list file - lives alongside this script in tools/
IGNORE_FILE="$(dirname "${0}")/find_build_warnings_ignore.conf"

# ---------------------------------------------------------------
# Resolve log directory
# If not passed in, source config/options to get $BUILD
# ---------------------------------------------------------------
if [ -n "${1}" ]; then
    LOGS_DIR="${1}"
else
    BUILD=$(. ./config/options 2>/dev/null; echo "${BUILD}")
    if [ -z "${BUILD}" ]; then
        echo "Error: Could not determine build directory from ./config/options"
        echo "Usage: tools/find_build_warnings.sh [logdir] [all|errors|warnings]"
        exit 1
    fi
    LOGS_DIR="${BUILD}/.threads/logs"
fi

if [ ! -d "${LOGS_DIR}" ]; then
    echo "Error: Log directory not found: ${LOGS_DIR}"
    echo "Usage: tools/find_build_warnings.sh [logdir] [all|errors|warnings]"
    exit 1
fi

# ---------------------------------------------------------------
# Parse ignore file
# Format:
#   -Wflag                          <- ignore this warning type globally
#   packagename:version             <- ignore this package at this version
#   packagename:version:message     <- same, with a reason message
# Lines starting with # are comments, blank lines ignored
# ---------------------------------------------------------------
IGNORE_WARNINGS=()
IGNORE_PACKAGES=()

if [ -f "${IGNORE_FILE}" ]; then
    while IFS= read -r line; do
        line=$(echo "${line}" | sed 's/#.*//' | sed 's/^[[:space:]]*//;s/[[:space:]]*$//')
        [ -z "${line}" ] && continue

        if [[ "${line}" == -W* ]]; then
            IGNORE_WARNINGS+=("${line}")
        elif [[ "${line}" == *:* ]]; then
            IGNORE_PACKAGES+=("${line}")
        fi
    done < "${IGNORE_FILE}"
fi

# Build warning ignore pattern
ignore_pattern=""
if [ ${#IGNORE_WARNINGS[@]} -gt 0 ]; then
    ignore_pattern=$(printf '%s\n' "${IGNORE_WARNINGS[@]}" \
        | sed 's/[][\\.^$*+?{}()|]/\\&/g' \
        | paste -sd'|' -)
fi

echo "Scanning logs in: ${LOGS_DIR}  (mode: ${MODE})"
echo "=========================================="

REPORT=$(mktemp)
WARN_SUMMARY=$(mktemp)
CLEANED=$(mktemp)
VERSION_CHANGED=$(mktemp)
IGNORED_LIST=$(mktemp)

for log in "${LOGS_DIR}"/*.log; do
    [ -f "${log}" ] || continue
    filename=$(basename "${log}")
    lognum="${filename%.log}"

    # Strip ANSI colour codes and carriage returns into clean working copy
    sed -E 's/\x1b\[[0-9;]*m//g; s/\r//' "${log}" > "${CLEANED}"

    # Extract package name
    pkg=$(grep -m1 -E "^\s*(UNPACK|BUILD)\s+" "${CLEANED}" 2>/dev/null \
          | sed -E 's/^\s*(UNPACK|BUILD)\s+([a-zA-Z0-9_+-]+).*/\2/')
    pkg="${pkg:-unknown}"

    # Pkg+log label used throughout report e.g. "hyperion (702)"
    pkglabel="${pkg} (${lognum})"

    # ---------------------------------------------------------------
    # Package ignore check - source config/options to get PKG_VERSION
    # ---------------------------------------------------------------
    skip_pkg=0
    if [ "${pkg}" != "unknown" ] && [ ${#IGNORE_PACKAGES[@]} -gt 0 ]; then
        pkg_version=$(. ./config/options "${pkg}" 2>/dev/null; echo "${PKG_VERSION}")

        for entry in "${IGNORE_PACKAGES[@]}"; do
            ignored_pkg=$(echo "${entry}" | cut -d: -f1)
            ignored_ver=$(echo "${entry}" | cut -d: -f2)
            ignored_msg=$(echo "${entry}" | cut -d: -f3-)

            if [ "${pkg}" = "${ignored_pkg}" ]; then
                if [ "${pkg_version}" = "${ignored_ver}" ]; then
                    skip_pkg=1
                    echo "${pkglabel}|${ignored_ver}|${ignored_msg}" >> "${IGNORED_LIST}"
                else
                    echo "${pkglabel}|${ignored_ver}|${pkg_version}|${ignored_msg}" >> "${VERSION_CHANGED}"
                fi
                break
            fi
        done
    fi

    [ ${skip_pkg} -eq 1 ] && continue

    # --- Errors ---
    has_errors=0
    if [[ "${MODE}" == "all" || "${MODE}" == "errors" ]]; then
        if grep -qiE "(make\[.*\]: \*\*\*|configure: error|CMake Error|ninja: build stopped)" "${CLEANED}" 2>/dev/null; then
            has_errors=1
        fi
    fi

    # --- Warnings ---
    warning_blocks=""
    if [[ "${MODE}" == "all" || "${MODE}" == "warnings" ]]; then
        warning_blocks=$(grep -nE "^(\.\./|\./)?[^:]+\.(c|h|cpp|hpp|cc|cxx)[^:]*:[0-9]+:[0-9]+: warning:" "${CLEANED}" 2>/dev/null)

        if [ -n "${ignore_pattern}" ] && [ -n "${warning_blocks}" ]; then
            warning_blocks=$(echo "${warning_blocks}" | grep -vE "${ignore_pattern}")
        fi
    fi

    [ ${has_errors} -eq 0 ] && [ -z "${warning_blocks}" ] && continue

    {
        echo ""
        echo "##########################################################"
        echo "## Log: ${filename}   Package: ${pkglabel}"
        echo "##########################################################"

        if [ ${has_errors} -eq 1 ]; then
            echo ""
            echo "  [ ERRORS ]"
            grep -iE "(make\[.*\]: \*\*\*|configure: error|CMake Error|ninja: build stopped)" \
                "${CLEANED}" | sed 's/^/  /'
        fi

        if [ -n "${warning_blocks}" ]; then
            echo ""
            echo "  [ WARNINGS ]"

            prev_func=""
            while IFS= read -r wline; do
                log_lineno=$(echo "${wline}" | cut -d: -f1)

                # Extract -Wflag and record for summary
                wflag=$(echo "${wline}" | grep -oE '\[-W[^]]+\]' | head -1)
                if [ -n "${wflag}" ]; then
                    echo "${wflag}|${pkglabel}" >> "${WARN_SUMMARY}"
                fi

                # Look back up to 5 lines for "In function" header
                func_line=$(awk "NR>=$((log_lineno-5)) && NR<${log_lineno} && /In function/" "${CLEANED}" 2>/dev/null | tail -1)

                if [ -n "${func_line}" ] && [ "${func_line}" != "${prev_func}" ]; then
                    echo ""
                    echo "    ${func_line}"
                    prev_func="${func_line}"
                fi

                # Print warning line (strip grep line number prefix)
                echo "    $(echo "${wline}" | cut -d: -f2-)"

                # Print source snippet (| ^ ~ lines)
                awk "NR==$((log_lineno+1)),NR==$((log_lineno+3))" "${CLEANED}" 2>/dev/null \
                    | grep -E "(\||~|\^)" \
                    | sed 's/^/      /'

            done <<< "${warning_blocks}"
        fi

    } >> "${REPORT}"
done

rm -f "${CLEANED}"

# ---------------------------------------------------------------
# Output report
# ---------------------------------------------------------------
if [ ! -s "${REPORT}" ] && [ ! -s "${WARN_SUMMARY}" ] && [ ! -s "${VERSION_CHANGED}" ]; then
    echo ""
    echo "No issues found."
else
    cat "${REPORT}"

    # ---------------------------------------------------------------
    # Warning type summary
    # ---------------------------------------------------------------
    echo ""
    echo "=========================================="
    echo "WARNING TYPE SUMMARY"
    echo "=========================================="

    if [ -s "${WARN_SUMMARY}" ]; then
        echo ""
        printf "  %-40s %6s  %s\n" "WARNING FLAG"  "COUNT" "PACKAGES AFFECTED"
        printf "  %-40s %6s  %s\n" "------------"  "-----" "-----------------"

        cut -d'|' -f1 "${WARN_SUMMARY}" | sort | uniq -c | sort -rn | \
        while read -r count wflag; do
            pkgs=$(grep -F "${wflag}|" "${WARN_SUMMARY}" | cut -d'|' -f2 | sort -u | paste -sd',' -)
            printf "  %-40s %6s  %s\n" "${wflag}" "${count}" "${pkgs}"
        done
    else
        echo "  No warnings recorded."
    fi

    # ---------------------------------------------------------------
    # Ignored packages listing
    # ---------------------------------------------------------------
    if [ -s "${IGNORED_LIST}" ]; then
        echo ""
        echo "=========================================="
        echo "IGNORED PACKAGES (version unchanged)"
        echo "=========================================="
        echo ""
        printf "  %-25s %-15s %s\n" "PACKAGE (LOG)"  "VERSION" "REASON"
        printf "  %-25s %-15s %s\n" "-------------"  "-------" "------"
        while IFS='|' read -r pkglabel ver msg; do
            printf "  %-25s %-15s %s\n" "${pkglabel}" "${ver}" "${msg}"
        done < "${IGNORED_LIST}"
    fi

    # ---------------------------------------------------------------
    # Version-changed notices
    # ---------------------------------------------------------------
    if [ -s "${VERSION_CHANGED}" ]; then
        echo ""
        echo "=========================================="
        echo "VERSION CHANGED - PREVIOUSLY IGNORED PACKAGES"
        echo "=========================================="
        echo ""
        printf "  %-25s %-15s %-15s %s\n" "PACKAGE (LOG)" "WAS IGNORED AT" "CURRENT VERSION" "REASON"
        printf "  %-25s %-15s %-15s %s\n" "-------------" "--------------" "---------------" "------"
        while IFS='|' read -r pkglabel old_ver new_ver msg; do
            printf "  %-25s %-15s %-15s %s\n" "${pkglabel}" "${old_ver}" "${new_ver}" "${msg}"
        done < "${VERSION_CHANGED}"
        echo ""
        echo "  Update or remove these entries in: ${IGNORE_FILE}"
    fi

    # ---------------------------------------------------------------
    # Hints
    # ---------------------------------------------------------------
    echo ""
    echo "=========================================="
    echo "  To ignore a warning type or package add to: ${IGNORE_FILE}"
    echo "  Warning type:  -Wunused-but-set-variable"
    echo "  Package:       hyperion:2.0.16"
    echo "  Package+note:  hyperion:2.0.16:fixed upstream, waiting for 2.0.17"

    # ---------------------------------------------------------------
    # Final summary line
    # ---------------------------------------------------------------
    echo ""
    echo "=========================================="
    total_pkgs=$(grep -c "^## Log:" "${REPORT}" 2>/dev/null || echo 0)
    total_warns=$(wc -l < "${WARN_SUMMARY}" 2>/dev/null || echo 0)
    total_warn_types=$(cut -d'|' -f1 "${WARN_SUMMARY}" | sort -u | wc -l 2>/dev/null || echo 0)
    total_errs=$(grep -c "\[ ERRORS \]" "${REPORT}" 2>/dev/null || echo 0)
    total_ignored=$(wc -l < "${IGNORED_LIST}" 2>/dev/null || echo 0)
    total_ver_changed=$(wc -l < "${VERSION_CHANGED}" 2>/dev/null || echo 0)
    echo "SUMMARY: ${total_pkgs} package(s) with issues — ${total_warns} warning(s) across ${total_warn_types} type(s), ${total_errs} error(s), ${total_ignored} ignored, ${total_ver_changed} version-changed notice(s)"
fi

rm -f "${REPORT}" "${WARN_SUMMARY}" "${VERSION_CHANGED}" "${IGNORED_LIST}"
