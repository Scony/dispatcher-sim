#!/bin/bash

[ $# -ne 3 ] && 1>&2 echo "arguments missing" && exit 1

inputDir="${1}"
outputDir="${2}"
machineFile="${3}"

algorithms=(random max min fifo sjlo sjso rvr dvr jsa sasa sjsa sa)
opts=(-eno -lmax -i100 -v2)

mkdir -p "${outputDir}"

for filePath in $(find "${inputDir}" -name '*.txt') ; do
    fileName="$(basename "${filePath}")"
    echo "${filePath}"
    for algorithm in "${algorithms[@]}" ; do
	echo "${algorithm}" "${opts[@]}"
	for i in $(seq 1 4) ; do
	    outputFilePath="${outputDir}/${algorithm}-${i}-${fileName}"
	    if [ ! -f "${outputFilePath}" ] || [ "$(du "${outputFilePath}" | cut -f 1)" == "0" ] ; then
		cat "${filePath}" "${machineFile}" | ./bin/simulator "${algorithm}" "${opts[@]}" 2>/dev/null | ./bin/stat-desc > "${outputFilePath}" &
	    fi
	done
	wait
	for i in $(seq 5 8) ; do
	    outputFilePath="${outputDir}/${algorithm}-${i}-${fileName}"
	    if [ ! -f "${outputFilePath}" ] || [ "$(du "${outputFilePath}" | cut -f 1)" == "0" ] ; then
		cat "${filePath}" "${machineFile}" | ./bin/simulator "${algorithm}" "${opts[@]}" 2>/dev/null | ./bin/stat-desc > "${outputFilePath}" &
	    fi
	done
	wait
    done
done
