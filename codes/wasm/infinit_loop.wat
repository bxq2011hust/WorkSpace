(module
  (type $t0 (func))
  (type $t1 (func (param i32 i32 i32 i32)))
  (import "bcos" "setStorage" (func $bcos.setStorage (type $t1)))
  (func $deploy (type $t0)
    (local $l0 i32)
      loop $L0
          local.get $l0
          i32.const 1
          i32.add
          local.get $l0
          i32.const 2
          i32.add
          local.get $l0
          i32.const 3
          i32.add
          local.get $l0
          call $bcos.setStorage
          br $L0
      end)
  (func $main (type $t0)
    (local $l0 i32)
    local.get $l0
    drop)
  (memory $M0 2)
  (export "deploy" (func $deploy))
  (export "main" (func $main)))
  ;; (export "memory" (memory 0)))