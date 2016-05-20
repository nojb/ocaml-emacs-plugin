/* The MIT License (MIT)

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
   SOFTWARE. */

#include <caml/memory.h>
#include <caml/mlvalues.h>
#include <caml/alloc.h>
#include <caml/callback.h>
#include <caml/fail.h>
#include <caml/custom.h>

#include <emacs-module.h>

int plugin_is_GPL_compatible;

static struct custom_operations emacs_value_custom_ops = {
  .identifier = "emacs_value",
  .finalize = custom_finalize_default,
  .compare = custom_compare_default,
  .hash = custom_hash_default,
  .serialize = custom_serialize_default,
  .deserialize = custom_deserialize_default
};

#define Emacs_value_val(v)  (*(emacs_value *) Data_custom_val (v))

static value
alloc_emacs_value (emacs_value ev)
{
  CAMLparam0 ();
  CAMLlocal1 (v);
  v = caml_alloc_custom (&emacs_value_custom_ops, sizeof (emacs_value), 0, 1);
  Emacs_value_val (v) = ev;
  CAMLreturn (v);
}

static emacs_value
emacs_function_callback (emacs_env *e, ptrdiff_t nargs, emacs_value args[], void *data)
{
  CAMLparam0 ();
  CAMLlocal4 (argsv, tmp, vtmp, r);
  value env = (value) e;
  argsv = Val_emptylist;
  for (int i = nargs-1; i >= 0; i --) {
    tmp = caml_alloc (2, 0);
    vtmp = alloc_emacs_value (args[i]);
    Store_field (tmp, 0, vtmp);
    Store_field (tmp, 1, argsv);
    argsv = tmp;
  }
  r = caml_callback2 (*(value *) data, env, argsv);
  CAMLreturnT (emacs_value, Emacs_value_val (r));
}

CAMLprim value
stub_make_global_ref (value e, value v)
{
  CAMLparam2 (e, v);
  CAMLlocal1 (ev);
  emacs_env *env = (emacs_env *) e;
  ev = alloc_emacs_value (env->make_global_ref (env, Emacs_value_val (v)));
  CAMLreturn (ev);
}

CAMLprim value
stub_free_global_ref (value e, value v)
{
  CAMLparam2 (e, v);
  emacs_env *env = (emacs_env *) e;
  env->free_global_ref (env, Emacs_value_val (v));
  CAMLreturn (Val_unit);
}

CAMLprim value
stub_make_function (value e, value min_arity, value max_arity, value f, value doc)
{
  CAMLparam5 (e, min_arity, max_arity, f, doc);
  CAMLlocal1 (v);
  emacs_env *env = (emacs_env *) e;
  value *root = malloc (sizeof (value));
  *root = f;
  caml_register_generational_global_root (root);
  v = alloc_emacs_value
    (env->make_function (env, Int_val (min_arity), Int_val (max_arity), emacs_function_callback, String_val (doc), root));
  CAMLreturn (v);
}

CAMLprim value
stub_funcall (value e, value f, value args)
{
  CAMLparam3 (e, f, args);
  CAMLlocal2 (v, ml);
  emacs_env *env = (emacs_env *) e;
  int nargs = 0;
  ml = args;
  while (ml != Val_emptylist) {
    nargs ++;
    ml = Field (ml, 1);
  }
  emacs_value *eargs = calloc (nargs, sizeof (emacs_value));
  ml = args;
  for (int i = 0; i < nargs; i ++) {
    eargs[i] = Emacs_value_val (Field (ml, 0));
    ml = Field (ml, 1);
  }
  emacs_value ev = env->funcall (env, Emacs_value_val (f), nargs, eargs);
  free (eargs);
  v = alloc_emacs_value (ev);
  CAMLreturn (v);
}

CAMLprim value
stub_intern (value e, value s)
{
  CAMLparam2 (e, s);
  CAMLlocal1 (v);
  emacs_env *env = (emacs_env *) e;
  v = alloc_emacs_value (env->intern (env, String_val (s)));
  CAMLreturn (v);
}

CAMLprim value
stub_type_of (value e, value v)
{
  CAMLparam2 (e, v);
  CAMLlocal1 (r);
  emacs_env *env = (emacs_env *) e;
  r = alloc_emacs_value (env->type_of (env, Emacs_value_val (v)));
  CAMLreturn (r);
}

CAMLprim value
stub_is_not_nil (value e, value v)
{
  CAMLparam2 (e, v);
  emacs_env *env = (emacs_env *) e;
  CAMLreturn (Val_bool (env->type_of (env, Emacs_value_val (v))));
}

CAMLprim value
stub_eq (value e, value v1, value v2)
{
  CAMLparam3 (e, v1, v2);
  emacs_env *env = (emacs_env *) e;
  CAMLreturn (Val_bool (env->eq (env, Emacs_value_val (v1), Emacs_value_val (v2))));
}

CAMLprim value
stub_extract_integer (value e, value v)
{
  CAMLparam2 (e, v);
  emacs_env *env = (emacs_env *) e;
  CAMLreturn (Val_int (env->extract_integer (env, Emacs_value_val (v))));
}

CAMLprim value
stub_make_integer (value e, value v)
{
  CAMLparam2 (e, v);
  CAMLlocal1 (ev);
  emacs_env *env = (emacs_env *) e;
  ev = alloc_emacs_value (env->make_integer (env, Int_val (v)));
  CAMLreturn (ev);
}

CAMLprim value
stub_extract_float (value e, value v)
{
  CAMLparam2 (e, v);
  CAMLlocal1 (d);
  emacs_env *env = (emacs_env *) e;
  d = caml_copy_double (env->extract_float (env, Emacs_value_val (v)));
  CAMLreturn (d);
}


CAMLprim value
stub_make_float (value e, value v)
{
  CAMLparam2 (e, v);
  CAMLlocal1 (ev);
  emacs_env *env = (emacs_env *) e;
  ev = alloc_emacs_value (env->make_float (env, Double_val (v)));
  CAMLreturn (ev);
}

CAMLprim value
stub_extract_string (value e, value v)
{
  CAMLparam2 (e, v);
  CAMLlocal1 (res);
  emacs_env *env = (emacs_env *) e;
  long len = 0;
  env->copy_string_contents (env, Emacs_value_val (v), NULL, &len);
  res = caml_alloc_string (len-1);
  env->copy_string_contents (env, Emacs_value_val (v), String_val(res), &len);
  CAMLreturn (res);
}

CAMLprim value
stub_make_string (value e, value s)
{
  CAMLparam2 (e, s);
  CAMLlocal1 (ev);
  emacs_env *env = (emacs_env *) e;
  ev = alloc_emacs_value (env->make_string (env, String_val (s), caml_string_length (s)));
  CAMLreturn (ev);
}

CAMLprim value
stub_vec_get (value e, value v, value i)
{
  CAMLparam3 (e, v, i);
  CAMLlocal1 (ev);
  emacs_env *env = (emacs_env *) e;
  ev = alloc_emacs_value (env->vec_get (env, Emacs_value_val (v), Int_val (i)));
  CAMLreturn (ev);
}

CAMLprim value
stub_vec_set (value e, value v, value i, value x)
{
  CAMLparam4 (e, v, i, x);
  emacs_env *env = (emacs_env *) e;
  env->vec_set (env, Emacs_value_val (v), Int_val (i), Emacs_value_val (x));
  CAMLreturn (Val_unit);
}

CAMLprim value
stub_vec_size (value e, value v)
{
  CAMLparam2 (e, v);
  emacs_env *env = (emacs_env *) e;
  int sz = env->vec_size (env, Emacs_value_val (v));
  CAMLreturn (Val_int (sz));
}

int
emacs_module_init (struct emacs_runtime *ert)
{
  char *argv[] = { "", NULL };
  caml_main (argv);
  callback (*caml_named_value ("emacs_init"), (value) ert->get_environment (ert));
  return 0;
}
