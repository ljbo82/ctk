# This file is part of ctk
# Copyright (C) 2021 - Leandro José Britto de Oliveira
# 
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
# 
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
# 
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA

CC := gcc

target   := ctk
buildDir := build
sources  := src/ctk/exception.c src/main.c
objs     := $(sources:%=$(buildDir)/%.o)
deps     := $(objs:.o=.d)

CFLAGS += -Iinclude -MMD -MP -pthread
LDFLAGS += -pthread

.PHONY: all
all: $(buildDir)/$(target)

.PHONY: clean
clean:
	rm -rf $(buildDir)

$(buildDir)/$(target): $(objs)
	$(CC) $(objs) -o $@ $(LDFLAGS)

$(buildDir)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

-include $(deps)