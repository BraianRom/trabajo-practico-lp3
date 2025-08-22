# ================================
# CONFIGURACIÓN
# ================================
SRC=../src
BIN=../bin

CC=g++
CFLAGS=-Wall -g

CAPS=cap1 cap2 cap3 cap4 cap5

# ================================
# TARGETS PRINCIPALES
# ================================
.PHONY: all clean $(CAPS)

# Compilar TODO
all: $(CAPS)

# Compilar un capítulo entero
$(CAPS):
	@mkdir -p $(BIN)/$@
	$(CC) $(CFLAGS) $(SRC)/$@/*.c -o $(BIN)/$@/main

# ================================
# COMPILAR UN LISTING PUNTUAL
# Uso: make cap1-listing1
# ================================
%:
	@chapter=$$(echo $@ | cut -d'-' -f1); \
	listing=$$(echo $@ | cut -d'-' -f2); \
	if [ -f $(SRC)/$$chapter/$$listing.c ]; then \
	  mkdir -p $(BIN)/$$chapter; \
	  $(CC) $(CFLAGS) $(SRC)/$$chapter/$$listing.c -o $(BIN)/$$chapter/$$listing; \
	  echo "✅ Compilado: $(BIN)/$$chapter/$$listing"; \
	else \
	  echo "No existe el archivo $(SRC)/$$chapter/$$listing.c"; \
	fi

# ================================
# LIMPIEZA
# ================================
clean:
	rm -rf $(BIN)/*

# Ejemplo: make clean-cap1 → borra solo bin/cap1
clean-%:
	rm -rf $(BIN)/$*
