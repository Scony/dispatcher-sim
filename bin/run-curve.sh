#!/bin/bash

[ $# -ne 2 ] && 1>&2 echo "arguments missing" && exit 1

inputDir="${1}"
outputDir="${2}"
consoleOutput="run.stderr"

machines=(30 50 100 200 400 800)
iterations=(0 1 10 100 1000 10000 100000)

mkdir -p "${outputDir}"
rm -f "${consoleOutput}"

for filePath in $(find "${inputDir}" -name '*.txt') ; do
    fileName="$(basename "${filePath}")"
    echo "${filePath}"
    for m in "${machines[@]}" ; do
        outputFilePath="${outputDir}/sjlo-x1-m${m}-eno-rqueue-${fileName}"
        echo "${outputFilePath}"
        if [ ! -f "${outputFilePath}" ] || [ "$(du "${outputFilePath}" | cut -f 1)" == "0" ] ; then
	    cat "${filePath}" | ./bin/simulator "sjlo" "-m${m}" "-eno" "-rqueue" 2>>"${consoleOutput}" | ./bin/stat-desc > "${outputFilePath}" &
	fi
    done
    wait
done

for filePath in $(find "${inputDir}" -name '*.txt') ; do
    fileName="$(basename "${filePath}")"
    echo "${filePath}"
    for i in "${iterations[@]}" ; do
        for m in "${machines[@]}" ; do
            for x in $(seq 1 7) ; do
	        outputFilePath="${outputDir}/sa-x${x}-m${m}-i${i}-eno-rschedule-${fileName}"
                echo "${outputFilePath}"
	        if [ ! -f "${outputFilePath}" ] || [ "$(du "${outputFilePath}" | cut -f 1)" == "0" ] ; then
	            cat "${filePath}" | ./bin/simulator "sa" "-m${m}" "-i${i}" "-eno" "-rschedule" 2>>"${consoleOutput}" | ./bin/stat-desc > "${outputFilePath}" &
	        fi
            done
            wait
        done
    done
done
