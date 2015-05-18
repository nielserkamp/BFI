
# This file is part of minemu
#
# Copyright 2010-2011 Erik Bosman <erik@minemu.org>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

.text

.global syscall0
.type syscall0, @function
syscall0:
push %ebp
movl %esp, %ebp
movl  8(%ebp),%eax
int $0x80
pop %ebp
ret

.global syscall1
.type syscall1, @function
syscall1:
push %ebp
movl %esp, %ebp
push %ebx
movl  8(%ebp),%eax
movl 12(%ebp),%ebx
int $0x80
pop %ebx
pop %ebp
ret

.global syscall2
.type syscall2, @function
syscall2:
push %ebp
movl %esp, %ebp
push %ebx
movl  8(%ebp),%eax
movl 12(%ebp),%ebx
movl 16(%ebp),%ecx
int $0x80
pop %ebx
pop %ebp
ret

.global syscall3
.type syscall3, @function
syscall3:
push %ebp
movl %esp, %ebp
push %ebx
movl  8(%ebp),%eax
movl 12(%ebp),%ebx
movl 16(%ebp),%ecx
movl 20(%ebp),%edx
int $0x80
pop %ebx
pop %ebp
ret

.global syscall4
.type syscall4, @function
syscall4:
push %ebp
movl %esp, %ebp
push %ebx
push %esi
movl  8(%ebp),%eax
movl 12(%ebp),%ebx
movl 16(%ebp),%ecx
movl 20(%ebp),%edx
movl 24(%ebp),%esi
int $0x80
pop %esi
pop %ebx
pop %ebp
ret

.global syscall5
.type syscall5, @function
syscall5:
push %ebp
movl %esp, %ebp
push %ebx
push %esi
push %edi
movl  8(%ebp),%eax
movl 12(%ebp),%ebx
movl 16(%ebp),%ecx
movl 20(%ebp),%edx
movl 24(%ebp),%esi
movl 28(%ebp),%edi
int $0x80
pop %edi
pop %esi
pop %ebx
pop %ebp
ret

.global syscall6
.type syscall6, @function
syscall6:
push %ebp
movl %esp, %ebp
push %ebx
push %esi
push %edi
movl  8(%ebp),%eax
movl 12(%ebp),%ebx
movl 16(%ebp),%ecx
movl 20(%ebp),%edx
movl 24(%ebp),%esi
movl 28(%ebp),%edi
movl 32(%ebp),%ebp
int $0x80
pop %edi
pop %esi
pop %ebx
pop %ebp
ret

.global prog_start
.type prog_start, @function
prog_start: # (long eip, long esp)
movl 4(%esp), %eax               # load user %eip from arg1
movl 8(%esp), %esp               # load user's stack pointer from arg2
push %eax
xor %eax, %eax                   # clear registers and flags
xor %ecx, %ecx
xor %edx, %edx
xor %ebx, %ebx
xor %ebp, %ebp
xor %esi, %esi
xor %edi, %edi
ret
