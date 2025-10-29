#!/bin/sh

PAYLOAD=

if [ -f "$1" ]; then
    PAYLOAD=$1
else
    echo "Usage: ./mk_efi_img.sh [ArceOS-Image-File.bin]"
    echo "Check for existence of the image file."
    exit 1
fi

make

./mk_img/mk_img /tmp/arceos ./efi_stub/efi_stub $PAYLOAD
