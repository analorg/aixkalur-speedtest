#!/bin/bash
start=$(date "+%s%6N")

export LANG=C.UTF-8

if [ $# -lt 2 ]; then
    program=$(basename "$0")
    echo "Usage: ./${program} /path/to/dictionary word"
    exit 1
fi

fileName=$1
word=$2
#wordLowered=$(echo "$word" | tr '[:upper:]' '[:lower:]')
wordLowered=$word

function strpos {
    needle=$1
    haystack=$2
    pos=-1
    for j in $(seq 1 ${#haystack}); do
        if [ "${haystack:j-1:1}" ==  "$needle" ]; then
            pos=$j
            break
        fi
    done

   echo $pos
}

found=""
IFS=$'\r\n'
for next in `cat $fileName`; do
    if [ ${#next} != ${#word} ]; then
        continue
    fi
    if [ ${next} == ${word} ]; then
        continue
    fi

    encoded=$(echo $next | iconv -f windows-1257 -t utf8)
    #lowered=$(echo "$encoded" | tr '[:upper:]' '[:lower:]')
    lowered=$encoded
    tempWord=$wordLowered

    for i in $(seq 1 ${#lowered}); do
        pos=$(strpos ${lowered:i-1:1} $tempWord)
        if [ "$pos" != "-1" ]; then
            startPos=$((pos-1))
            tempWord=${tempWord:0:startPos}${tempWord:pos}
        else
            break
        fi
    done
    if [ "$tempWord" == "" ]; then
        found="$found, $encoded"
    fi
done

found="${found:1}"

end=$(date "+%s%6N")
runtimeMicro=$((end-start))
runtime=$((runtimeMicro / 1000000))
separator=""
if [ "${found}" != "" ]; then
    separator=","    
fi

echo "${runtimeMicro}${separator}${found}"
