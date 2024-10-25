# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Cub3d.mk                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: maiboyer <maiboyer@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/11/03 13:20:01 by maiboyer          #+#    #+#              #
#    Updated: 2024/10/22 14:19:41 by maiboyer         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

ANAME			=	c3

BUILD_DIR		?=	../build
SRC_DIR			=	src
INCLUDE_DIR		=	include stdme/include
LIBS_DIR		=	.
GEN_DIR			=	output/src
GEN_INCLUDE		=	output/include stdme/output/include


BASE_PATH		?=	$(shell pwd)
NAME			=	cub3d
LIB_NAME		?=	
TARGET			=	$(NAME)
CC				?=	cc
CFLAGS			=	-Wall -Werror -Wextra -MMD -DBASE_PATH='"$(BASE_PATH)/"'
CFLAGS			+=	$(CFLAGS_ADDITIONAL)
CLDFLAGS		=	-L$(BUILD_DIR) -lme -lmlx -lX11 -lXext -lm

-include 			./Filelist.$(ANAME).mk

OBJ				=	$(addsuffix .o,$(addprefix $(BUILD_DIR)/$(ANAME)/,$(SRC_FILES) $(GEN_FILES)))
DEPS			=	$(addsuffix .d,$(addprefix $(BUILD_DIR)/$(ANAME)/,$(SRC_FILES) $(GEN_FILES)))

INCLUDES		=	$(addprefix -I,$(foreach P,$(INCLUDE_DIR) $(GEN_INCLUDE), $(realpath $(P))))
COL_GRAY		=	\033[90m
COL_WHITE		=	\033[37m
COL_GREEN		=	\033[32m
COL_BOLD		=	\033[1m
COL_RESET		=	\033[0m
COL_GOLD		=	\033[93m


.PHONY = all bonus clean re subject make_libs

all: $(NAME)

make_libs: 
	@$(MAKE) -C ./stdme/ 	"LIB_NAME=$(shell realpath ./stdme)/"	libme.a
	@$(MAKE) -C ./mlx/ -f Makefile.mk "CFLAGS=-O3 $(CFLAGS_ADDITIONAL)" "OBJ_DIR=$(BUILD_DIR)/mlx" "NAME=$(BUILD_DIR)/libmlx.a"

$(TARGET): $(OBJ) $(BUILD_DIR)/libme.a $(BUILD_DIR)/libmlx.a
	@echo -e '$(COL_GRAY) Linking \t$(COL_GOLD)$(TARGET)$(COL_RESET)'
	@$(CC) $(CFLAGS) -Wl,--start-group  $(CLDFLAGS) $(OBJ) -Wl,--end-group   -o $(NAME)
	@#ar rcs $(BUILD_DIR)/$(NAME) $(OBJ)

$(BUILD_DIR)/$(ANAME)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo -e '$(COL_GRAY) Building\t$(COL_GREEN)$<$(COL_RESET)'
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILD_DIR)/$(ANAME)/%.o: $(GEN_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo -e '$(COL_GRAY) Building\t$(COL_GREEN)$<$(COL_RESET)'
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo -e '$(COL_WHITE) Deleting\t$(COL_RED)$(BUILD_DIR)$(COL_RESET)'
	@rm -f $(BUILD_DIR)/$(NAME)

fclean: clean
	@echo -e '$(COL_WHITE) Deleting\t$(COL_RED)$(NAME)$(COL_RESET)'
	@rm -f $(NAME)

re: fclean all

subject: subject.txt
	bat --plain ./subject.txt

subject.txt:
	@curl $(SUBJECT_URL) | pdftotext -layout -nopgbrk -q - subject.txt


filelist:
	@echo -e "$(COL_RESET)Filelist for Cub3d"
	@rm -f Filelist.$(ANAME).mk
	@printf '%-78s\\\n' "SRC_FILES =" > Filelist.$(ANAME).mk
	@tree $(SRC_DIR) -ifF | rg '$(SRC_DIR)/(.*)\.c$$' --replace '$$1' | sed -re 's/^(.*)_([0-9]+)$$/\1|\2/g' | sort -t'|' --key=1,1 --key=2,2n | sed -e's/|/_/' | xargs printf '%-78s\\\n' >> Filelist.$(ANAME).mk
	@echo "" >> Filelist.$(ANAME).mk
	@printf '%-78s\\\n' "GEN_FILES =" >> Filelist.$(ANAME).mk
	@tree $(GEN_DIR) -ifF | rg '$(GEN_DIR)/(.*)\.c$$' --replace '$$1' | sed -re 's/^(.*)_([0-9]+)$$/\1|\2/g' | sort -t'|' --key=1,1 --key=2,2n | sed -e's/|/_/' | xargs printf '%-78s\\\n' >> Filelist.$(ANAME).mk
	@echo "" >> Filelist.$(ANAME).mk

%.h: ;

-include $(DEPS)
