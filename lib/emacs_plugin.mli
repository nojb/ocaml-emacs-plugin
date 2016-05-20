(* The MIT License (MIT)

   Copyright (c) 2016 Nicolas Ojeda Bar <n.oje.bar@gmail.com>

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE. *)

(** Bindings to the Emacs loadable modules API

    Some remarks:

    - To get things started you need to register a callback (using [Callback.register])
      named ["emacs_init"] of type [env -> unit].  It is important for the call
      to [Callback.register] to appear on the module top-level.

      Inside the [emacs_init] function you can used the passed [env] to register
      the plugin's functions using [make_function].

    - [env] values are transient (stack-allocated).  Under no circumstances they
      should be saved inside the closure passed to [make_function] and re-used
      after it returns.

    - all [value] objects are garbage-collected when the function passed to
      [make-function] returns, except for the return value.  If you want to
      preserve a [value] longer than that, you need to use [make_global_ref] and
      [free_global_ref]. *)

type env
type value

val make_global_ref: env -> value -> value
val free_global_ref: env -> value -> unit
val make_function: env -> int -> int -> (env -> value list -> value) -> string -> value
val funcall: env -> value -> value list -> value
val intern: env -> string -> value
val type_of: env -> value -> value
val is_not_nil: env -> value -> bool
val eq: env -> value -> value -> bool
val extract_integer: env -> value -> int
val make_integer: env -> int -> value
val extract_float: env -> value -> float
val make_float: env -> float -> value
val extract_string: env -> value -> string
val make_string: env -> string -> value

(* val make_user: (module USER with type t = 'a) -> (value -> 'a option) * ('a -> value) *)

val vec_get: env -> value -> int -> value
val vec_set: env -> value -> int -> value -> unit
val vec_size: env -> value -> int
