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
