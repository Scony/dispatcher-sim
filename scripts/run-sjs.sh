#!/bin/bash

[ $# -ne 2 ] && 1>&2 echo "arguments missing" && exit 1

inputDir="${1}"
outputDir="${2}"

algorithms=(sj sjlo sjso sjrand sjmd sjmdr sjavg sjavgr)
machines=(30 50 100 200 400 800)

mkdir -p "${outputDir}"

for filePath in $(find "${inputDir}" -name '*.txt') ; do
    fileName="$(basename "${filePath}")"
    echo "${filePath}"
    for m in "${machines[@]}" ; do
        for algorithm in "${algorithms[@]}" ; do
	    echo "${algorithm}"
            runs=1
            [ "${algorithm}" == "sjrand" ] && runs=7
	    for x in $(seq 1 "${runs}") ; do
                outputFilePath="${outputDir}/${algorithm}-x${x}-m${m}-eno-rqueue-${fileName}"
	        if [ ! -f "${outputFilePath}" ] || [ "$(du "${outputFilePath}" | cut -f 1)" == "0" ] ; then
		    cat "${filePath}" | ./bin/simulator "${algorithm}" "-m${m}" "-eno" "-rqueue" 2>/dev/null | ./bin/stat-desc > "${outputFilePath}"
	        fi
	    done
        done
    done
done
