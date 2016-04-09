'use strict'

// Flash your photon with the example tinker script, and run this to blink D7
// https://build.particle.io/examples/56214d636666d9ece3000006

var sparkUtils = require('./lib/sparkUtils.js')

sparkUtils.loginAndGetDevice('p2').then(function(device) {
  console.log(device)
  function tick(state) {
    console.log(state)
    device.callFunction('digitalwrite', state ? 'D7:HIGH' : 'D7:LOW')
    setTimeout(tick.bind(null, !state), 3000)
  }

  tick(true)
})
