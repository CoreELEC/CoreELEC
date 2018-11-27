#!/bin/bash
cd /storage/.config/acelist
FNAME="acelive.json"
URL="http://91.92.66.82/trash/ttv-list/acelive.json"

GNAME="${FNAME}.gz"
curl --fail -R -z "${GNAME}" -o "${GNAME}" -H "Accept-Encoding: gzip" "${URL}" && \
 gunzip -c "${GNAME}" >"${FNAME}"

python3 /storage/.config/acelist/acelist.py


FNAME2="as.json"
URL2="http://91.92.66.82/trash/ttv-list/as.json"

GNAME2="${FNAME2}.gz"
curl --fail -R -z "${GNAME2}" -o "${GNAME2}" -H "Accept-Encoding: gzip" "${URL2}" && \
 gunzip -c "${GNAME2}" >"${FNAME2}"

python3 /storage/.config/acelist/aslist.py

