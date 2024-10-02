#!/bin/bash

i=0
while IFS=':' read -r login enc_pass uid gid user_name home shell;
do
    i=$((i+1));
    j=$(dirname "$home");

    # otra manera de hacerlo:
    #echo "$j";
    # if [[ "$j" =~ ^/home ]]; then

    if [[ "$home" =~ ^/home ]]; then #^ es bitwise XOR pero =~ compara regexs y ahi significa inicio de cadena
        echo -e "[Entry: $i]\n\tlogin=$login\n\tenc_pass=$enc_pass\n\tuid=$uid\n\tgid=$gid\n\tuser_name=$user_name\n\thome=$home\n\tshell=$shell\n";
    fi
done < /etc/passwd
grep '^.*:.*:.*:.*:.*:/home' /etc/passwd | cut -d':' -f6
