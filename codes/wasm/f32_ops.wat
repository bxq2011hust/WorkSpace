(module
  (type $t0 (func))
  (type $t1 (func (param i32 i32 i32 i32)))
  (type $t2 (func (param i32 i32)))
  (type $t3 (func (param f32) (result f32)))
  (type $t4 (func (param f32)))
  (import "bcos" "setStorage" (func $bcos.setStorage (type $t1)))
  (import "bcos" "revert" (func $bcos.revert (type $t2)))
  (import "bcos" "finish" (func $bcos.finish (type $t2)))
  (func $deploy (type $t0)
    f32.const 5.1
    call $add
    global.get $g0
    f32.sub
    f32.const 0.2
    f32.eq
    if $I0
      global.get $g0
      i32.trunc_f32_s
      i32.const 3
      call $bcos.finish
    else
      global.get $g0
      f32.abs
      i32.trunc_f32_s
      i32.const 2
      call $bcos.revert
    end)
  (func $main (type $t0)
    (local $l0 i32)
    local.get $l0
    drop)
  (func $add (type $t3) (param $p0 f32) (result f32)
    local.get $p0
    f32.const 5.2
    f32.add)
  (func $setGlobal (type $t4) (param $p0 f32)
    local.get $p0
    global.set $g0)
  (memory $M0 2)
  (global $g0 (mut f32) (f32.const 10.1))
  (export "deploy" (func $deploy))
  (export "main" (func $main)))
  ;; (export "memory" (memory 0)))