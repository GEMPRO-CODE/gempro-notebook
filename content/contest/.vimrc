syntax on
set cin ts=4 sw=4

ca Hash w !cpp -dD -P -fpreprocessed \| tr -d '[:space:]' \| md5sum \| cut -c -6
