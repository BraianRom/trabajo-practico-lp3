# Carpeta de código fuente y binarios
SRC_DIR = src
BIN_DIR = bin

# Listings de cada capítulo
CAP1_LISTINGS = 1.1 1.2 1.3
CAP2_LISTINGS = 2.1 2.2 2.3 2.4 2.5 2.6 2.7 2.8 2.9
CAP3_LISTINGS = 3.1 3.2 3.3 3.4 3.5 3.6 3.7 3.8
CAP4_LISTINGS = 4.1 4.2 4.3 4.4 4.5 4.6 4.7 4.8 4.9 4.10 4.11 4.12 4.13 4.14 4.15
CAP5_LISTINGS = 5.1 5.2 5.3 5.4 5.5 5.6 5.7 5.8 5.9 5.10 5.11 5.12

# Todos los listings
ALL_LISTINGS = $(CAP1_LISTINGS:%=% ) $(CAP2_LISTINGS:%=% ) $(CAP3_LISTINGS:%=% ) \
               $(CAP4_LISTINGS:%=% ) $(CAP5_LISTINGS:%=%)

# Compilador y flags
CC = gcc
CFLAGS = -Wall -pthread

# Default target
all: $(ALL_LISTINGS:%=listing-%)

# Regla para compilar un listing específico
listing-%:
	@echo "Compilando listing $*..."
	@mkdir -p $(BIN_DIR)/cap$$(echo $* | cut -d. -f1)
	$(CC) $(CFLAGS) $(SRC_DIR)/cap$$(echo $* | cut -d. -f1)/listing$*.c -o $(BIN_DIR)/cap$$(echo $* | cut -d. -f1)/listing$*

# Clean
clean:
	@echo "Eliminando todos los binarios..."
	rm -rf $(BIN_DIR)/*

.PHONY: all clean $(ALL_LISTINGS:%=listing-%)

