#!/bin/bash

# Mira si existe el directorio
if [ ! -d "$1" ]; then
    echo "El directorio no existe, Sorry :("
    exit 1
fi

# Ciclo para cambiar permisos
for i in "$1"/*; do
    if [ -f "$i" ]; then
        chmod 764 "$i"
    elif [ -d "$i" ]; then
        chmod 640 "$i"
    fi
done