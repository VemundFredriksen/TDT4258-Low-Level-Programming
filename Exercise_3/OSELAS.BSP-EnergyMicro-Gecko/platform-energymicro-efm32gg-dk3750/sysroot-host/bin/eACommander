#!/bin/sh

#
# Run Commander
#
DIR="$( cd "$( dirname "$0" )" && pwd )"
export LD_LIBRARY_PATH="${DIR}/../lib/energyAwareTools":/usr/lib32/gio/modules
"${DIR}/../lib/energyAwareTools/eACommander" --flashloader "${DIR}/../lib/energyAwareTools/nandflashloader.bin" "$@"
