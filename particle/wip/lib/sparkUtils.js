'use strict'

var spark = require('spark')

var loginParams = { accessToken: require('../accessToken.json') }

function listDevices() {
  return spark.listDevices()
}

function deviceGetter(deviceName) {
  return function(devices) {
    return devices.filter(function(device) {
      return device.attributes.name === deviceName
    })[0]
  }
}

/////////////////////////////////////////////

function loginAndGetDevice(name) {
  return spark.login(loginParams).then(listDevices).then(deviceGetter(name))
}

module.exports = {
  spark: spark,
  loginAndGetDevice: loginAndGetDevice  
}
