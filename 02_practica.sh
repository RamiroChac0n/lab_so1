#!/bin/bash

# Descargar el index.html de la USAC
wget https://usac.edu.gt/ -O index.html

# Mira si si se descargó
if [ ! -f "index.html" ]; then
    echo "Error: No se pudo descargar la página."
    exit 1
fi

# Copia para comparar
cp index.html index_original.html

# Antes de modificar
echo "Etiquetas antes:"
grep -o -E '<h[1-6]' index.html | sort | uniq -c

# Aplicando la modificación
sed -i -E 's/h5/h6/g; s/h4/h5/g; s/h3/h4/g; s/h2/h3/g; s/h1/h2/g' index.html

# Después de modificar
echo "Etiquetas despues:"
grep -o -E '<h[1-6]' index.html | sort | uniq -c
