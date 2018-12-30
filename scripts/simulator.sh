#!/bin/bash

mdir="$(dirname "$(readlink -f "$0")")"
"${mdir}/simulator" "${@}" | "${mdir}/stat-desc"
