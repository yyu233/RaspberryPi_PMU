cmd_/home/dev/RPdev/part1/pmuon/modules.order := {   echo /home/dev/RPdev/part1/pmuon/pmuon.ko; :; } | awk '!x[$$0]++' - > /home/dev/RPdev/part1/pmuon/modules.order
