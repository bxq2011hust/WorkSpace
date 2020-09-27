const fs = require('fs')
const metering = require('wasm-metering')

const rawWasm = 'clz32.wasm'

const wasm = fs.readFileSync(rawWasm)
const meteredWasm = metering.meterWASM(wasm, {
  meterType: 'i32'
})

const limit = 90000000
let gasUsed = 0

var mod = new WebAssembly.Module(meteredWasm)
var instance = new WebAssembly.Instance(mod, {
  'metering': {
    'usegas': (gas) => {
      gasUsed += gas
      if (gasUsed > limit) {
        throw new Error('out of gas!')
      }
    }
  }
})
fs.writeFileSync('meter_'+rawWasm,meteredWasm,{})
const result = instance.exports._clz_32(10)
console.log(`result:${result}, gas used ${gasUsed * 1e-4}`) // result:720, gas used 0.4177