const Addon = require('./build/Release/addon')

console.log('C++ Module',Addon)
console.log('Addon.hello(1, -1) === ', Addon.hello(1,-1))
const repetitions = 10000;
function hello(a,b) {
  if(isNaN(a) || isNaN(b)) throw new Error("Must have a number")
  return a + b
}
console.time("JS HELLO WORLD")
for (let i = 0; i < repetitions; i++) hello(Math.random(), Math.random())
console.timeEnd("JS HELLO WORLD")

console.time("C++ HELLO WORLD")
for (let i = 0; i < repetitions; i++) Addon.hello(Math.random(), Math.random())
console.timeEnd("C++ HELLO WORLD")

console.log("C++ Weight Library")
const tenMicroGrams = new Addon.microgram(10)
const fiveMicroGrams = new Addon.microgram(5)

const s = w => `${w.name}(${w.count})`;
console.log(`${s(tenMicroGrams)} + ${s(fiveMicroGrams)} === ${s(tenMicroGrams.add(fiveMicroGrams))}`)
