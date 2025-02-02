# **************************************************************************** #make
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rparodi <rparodi@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/12 11:05:05 by rparodi           #+#    #+#              #
#    Updated: 2024/11/22 15:20:11 by maiboyer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Objdir
BUILD_DIR = $(shell realpath ./build)
BASE_PATH = $(shell pwd)

CC=cc

# Colors
GREEN = \033[32m
CYAN = \033[36m
GREY = \033[0;90m
RED = \033[0;31m
GOLD = \033[38;5;220m
END = \033[0m
BOLD = \033[1m
ITALIC = \033[3m
UNDERLINE = \033[4m

CFLAGS_ADDITIONAL=
# PMAKE_DISABLE =
PMAKE =
ifndef PMAKE_DISABLE
ifeq ($(shell uname), Linux)
	PMAKE = -j$(shell grep -c ^processor /proc/cpuinfo)
#	CFLAGS_ADDITIONAL	+= -DPRINT_BACKTRACE
endif
ifeq ($(shell uname), Darwin)
	PMAKE = -j$(shell sysctl -n hw.ncpu)
	#CFLAGS_ADDITIONAL	+= -DNVALGRIND
endif
endif

# TODO: ADD THIS WHEN FINISHING THIS:
CFLAGS_ADDITIONAL	+= -DNVALGRIND


ifeq ($(MAKECMDGOALS), bonus)
    CFLAGS_ADDITIONAL += -DBONUS=1
    BUILD_DIR := $(BUILD_DIR)/bonus
endif

NAME=cub3D
# TODO: REMOVE THIS WHEN FINISHING THIS:
# CFLAGS_ADDITIONAL	+= -O0 -Wno-\#warnings 
# CFLAGS_ADDITIONAL	+= -gcolumn-info -g3 -fno-builtin
# CFLAGS_ADDITIONAL	+= '-DERROR=((void)printf("ERROR HERE: " __FILE__ ":%d in %s\n", __LINE__, __func__), 1)'
# CFLAGS_ADDITIONAL	+= -fsanitize=address

export CFLAGS_ADDITIONAL
export CC
export BASE_PATH
export BUILD_DIR


# All (make all)
all: ./mlx
	@$(MAKE) --no-print-directory header
	@$(MAKE) --no-print-directory -f ./Cub3d.mk $(PMAKE) make_libs
	@$(MAKE) --no-print-directory -f ./Cub3d.mk $(PMAKE)
	@$(MAKE) --no-print-directory footer

bonus: 
	@$(MAKE) --no-print-directory header
	@$(MAKE) --no-print-directory -f ./Cub3d.mk $(PMAKE) make_libs
	@$(MAKE) --no-print-directory -f ./Cub3d.mk $(PMAKE) bonus
	@$(MAKE) --no-print-directory footer

#	Header
header:
	@echo -e ''
	@echo -e '$(GOLD)            *******     ****** ******* $(END)'
	@echo -e '$(GOLD)          ******        ***    ******* $(END)'
	@echo -e '$(GOLD)      *******           *      ******* $(END)'
	@echo -e '$(GOLD)     ******                  *******   $(END)'
	@echo -e '$(GOLD)  *******                  *******     $(END)'
	@echo -e '$(GOLD) *******************    *******      * $(END)'
	@echo -e '$(GOLD) *******************    *******    *** $(END)'
	@echo -e '$(GOLD)              ******    ******* ****** $(END)'
	@echo -e '$(GOLD)              ******                   $(END)'
	@echo -e '$(GOLD)              ******                   $(END)'
	@echo -e '$(GREY)          Made by maiboyerlpb x lgasqui$(END)'

#	Footer
footer:
	@echo -e '$(GOLD)$(END)'
	@echo -e '$(GOLD)   +------+                 +------+   $(END)'
	@echo -e '$(GOLD)  /|     /|                 |\     |\  $(END)'
	@echo -e '$(GOLD) +-+----+ |                 | +----+-+ $(END)'
	@echo -e '$(GOLD) | |    | |      $(CYAN)$(BOLD)$(UNDERLINE)cub3d$(END)$(GOLD)      | |    | | $(END)'
	@echo -e '$(GOLD) | +----+-+                 +-+----+ | $(END)'
	@echo -e '$(GOLD) |/     |/                   \|     \| $(END)'
	@echo -e '$(GOLD) +------+                     +------+ $(END)'
	@echo -e '$(GOLD)$(END)'
	@echo -e '            $(GREY)The compilation is $(END)$(GOLD)finished$(END)'
	@echo -e '                 $(GREY)Have a good $(END)$(GOLD)correction$(END)'

./mlx:
	@git clone https://github.com/42Paris/minilibx-linux ./mlx

# Clean (make clean)
clean:
	@echo -e '$(GREY) Removing $(END)$(RED)Objects$(END)'
	@echo -e '$(GREY) Removing $(END)$(RED)Objects Folder$(END)'
	@$(RM) -r $(BUILD_DIR)

# Clean (make fclean)
fclean: clean
	@echo -e '$(GREY) Removing $(END)$(RED)Program$(END)'
	@$(RM) $(NAME)
	@$(RM) $(NAME)_bonus
	@echo ""

# Restart (make re)
re: 
	@$(MAKE) --no-print-directory fclean
	@$(MAKE) --no-print-directory all

tokei:
	@/bin/sh -c 'tokei -tC,C\ Header'


filelist:
	@$(MAKE) --no-print-directory -C ./stdme/ 	filelist
	@$(MAKE) --no-print-directory -f ./Cub3d.mk	filelist

#	phony
.PHONY: all bonus clean fclean re header footer filelist
