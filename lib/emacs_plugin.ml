type env
type value

external make_global_ref: env -> value -> value = "stub_make_global_ref"
external free_global_ref: env -> value -> unit = "stub_free_global_ref"
external make_function: env -> int -> int -> (env -> value list -> value) -> string -> value = "stub_make_function"
external funcall: env -> value -> value list -> value = "stub_funcall"
external intern: env -> string -> value = "stub_intern"
external type_of: env -> value -> value = "stub_type_of"
external is_not_nil: env -> value -> bool = "stub_is_not_nil"
external eq: env -> value -> value -> bool = "stub_eq"
external extract_integer: env -> value -> int = "stub_extract_integer"
external make_integer: env -> int -> value = "stub_make_integer"
external extract_float: env -> value -> float = "stub_extract_float"
external make_float: env -> float -> value = "stub_make_float"
external extract_string: env -> value -> string = "stub_extract_string"
external make_string: env -> string -> value = "stub_make_string"

(* val make_user: (module USER with type t = 'a) -> (value -> 'a option) * ('a -> value) *)

external vec_get: env -> value -> int -> value = "stub_vec_get"
external vec_set: env -> value -> int -> value -> unit = "stub_vec_set"
external vec_size: env -> value -> int = "stub_vec_size"
