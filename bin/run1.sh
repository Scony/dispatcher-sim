#!/bin/bash

[ $# -ne 2 ] && 1>&2 echo "arguments missing" && exit 1

inputDir="${1}"
outputDir="${2}"

algorithms=(random max min fifo sjlo sjso rvr dvr)
opts=(-m20 -eno)

mkdir -p "${outputDir}"

for filePath in $(find "${inputDir}" -name '*.txt') ; do
    fileName="$(basename "${filePath}")"
    echo "${filePath}"
    for algorithm in "${algorithms[@]}" ; do
	echo "${algorithm}" "${opts[@]}"
	for i in $(seq 1 10) ; do
	    outputFilePath="${outputDir}/${algorithm}-${i}-${fileName}"
	    if [ ! -f "${outputFilePath}" ] || [ "$(du "${outputFilePath}" | cut -f 1)" == "0" ] ; then
		cat "${filePath}" | ./bin/simulator "${algorithm}" "${opts[@]}" 2>/dev/null | ./bin/stat-desc > "${outputFilePath}"
	    fi
	done
    done
done
