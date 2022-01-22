cmd_/home/dev/RPdev/part1/cross_hello/modules.order := {   echo /home/dev/RPdev/part1/cross_hello/hello.ko; :; } | awk '!x[$$0]++' - > /home/dev/RPdev/part1/cross_hello/modules.order
