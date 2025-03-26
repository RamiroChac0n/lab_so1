#!/bin/bash

# Mira si existe el directorio
if [ ! -d "$1" ]; then
    echo "El directorio no existe, Sorry :("
    exit 1
fi

# Permisos para directorios
find "$1"/* -type d -exec chmod 640 {} \;

# Permisos para archivos
find "$1"/* -type f -exec chmod 764 {} \;