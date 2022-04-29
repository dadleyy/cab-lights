#!/bin/bash

if [[ -z `which arduino-cli` ]]; then
  echo "Unable to find 'arduino-cli'" >&2
  exit 1
fi

find_board() {
  local boards=`arduino-cli board list --format yaml | grep "address"`
  local result=""

  while IFS=' ' read -ra ADDR; do
    for i in "${ADDR[@]}"; do
      if [[ $i == "address:" ]]; then
        continue
      fi
      result=$i
    done
  done <<< "$boards"

  if [[ -z $result ]]; then
    echo "Unable to find a board" >&2
    exit 1
  fi

  if [[ $result == "/dev/cu.Bluetooth-Incoming-Port" ]]; then
    echo "Avoiding osx internal bluetooth port explicitly" >&2
    exit 1
  fi

  echo $result
}

find_board
