open Emacs_plugin

let bind_function env name f =
  let qfset = intern env "fset" in
  let qsym = intern env name in
  ignore (funcall env qfset [qsym; f])

let provide env feature =
  let qfeat = intern env feature in
  let qprovide = intern env "provide" in
  ignore (funcall env qprovide [qfeat])

let emacs_init env =
  bind_function env "anyone-there" (make_function env 0 0 (fun env _ -> intern env "hello") "foodoc");
  provide env "test_plugin"

let () =
  prerr_endline "test_plugin";
  Callback.register "emacs_init" emacs_init
