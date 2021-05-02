/*
     This file is part of ctk
     Copyright (C) 2021 - Leandro José Britto de Oliveira

     This library is free software; you can redistribute it and/or
     modify it under the terms of the GNU Lesser General Public
     License as published by the Free Software Foundation; either
     version 2.1 of the License, or (at your option) any later version.

     This library is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
     Lesser General Public License for more details.

     You should have received a copy of the GNU Lesser General Public
     License along with this library; if not, write to the Free Software
     Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#pragma once

#include <stddef.h>

typedef const char* cstring_t;

// Required to be 1 on windows, otherwise 0
#define CTK_EXCEPTION_ENABLE_SIGNAL_SUPPORT 1

#define CTK_EXCEPTION_ENABLE_THREAD_SUPPORT 0
