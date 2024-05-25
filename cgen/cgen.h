/*
 * Copyright (c) 2004, Anthony Roberts
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * * Neither the name of Anthony Roberts nor the names of other contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * These macros are used to implement generators similar to those in python.
 *
 * Variables accessed across invocations CAN NOT BE LOCAL to the generator
 * function. It is recommended that variables either be static locals if
 * reentrancy is not needed, or passed as a pointer to the function. It may be
 * more convenient to define a struct with fields that you can use in place
 * of local variables. Any resource allocations (file descriptors, memory
 * allocations) will leak if they are allocated but not freed or explicitly
 * retained across invocations.
 *
 * State should be initialized to CGEN_START, and will be equal to
 * CGEN_DONE when the code has reached the end of the resumable block.
 *
 * cgen_begin should be called before code that will be resumable. The argument
 * is the state variable. No return value should be assumed.
 *
 * cgen_yield will allow the function to return a value but allow the function
 * to resume immediately after that the next time it's called (excluding code
 * that comes before the cgen_begin macro). Please note that the macro uses
 * the line number to generate unique state labels, so multiple usages of
 * cgen_yield instances shouldn't be on the same line. The first argument is
 * the state variable. Because of the way the macro works, it must be a
 * variable that can be set. The second argument is the value to be passed
 * back to the caller.
 *
 * cgen_end should come after the code that will be resumable. Code can come
 * after it, and if it is reached the state will be set to return there
 * afterwards. This can be used to return a flag to indicate that the generator
 * is finished. The argument should be the state variable. Because of the way
 * the macro works, it must be a variable that can be set.
 */

#define cgen_begin(state) switch (state) { case 0:
#define cgen_yield(state, ret) { state = __LINE__; return(ret); \
 case __LINE__: {} }
#define cgen_end(state) state = -1; case -1: {} }

#define CGEN_START 0
#define CGEN_DONE -1
