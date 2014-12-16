'use strict'

// This was not meant to be easily readable.

function linear(value, x1, x2, y1, y2) {
  return (value - x1)*((y2 - y1)/(x2 - x1)) + y1;
}

function arrayOf(size) {
  return Array.apply(null, { length: size })
}

var data = require('fs').readFileSync('./data.txt').toString().replace(/(^{|}$)/g,'').split('}{').map(function(i){return '{'+i+'}'}).map(JSON.parse)

var minmax = {}

data.forEach(function(i) {
  Object.keys(i).forEach(function(type) {
    minmax[type] = minmax[type] || { min: Infinity, max: -Infinity }
    Object.keys(minmax[type]).forEach(function(f) {
      minmax[type][f] = Math[f](minmax[type][f], i[type])
    })
  })
}) //Muhahahaha


var size = {
  time: process.stdout.columns - 1,
  temp: process.stdout.rows - 1
}

var image = arrayOf(size.temp+1).map(function(){return arrayOf(size.time+1).map(function(){return ' '})})

data.forEach(function(i) {
  var coordinates = Object.keys(i).map(function(key) {
    return Math.floor(linear(i[key], minmax[key].min, minmax[key].max, 0, size[key]))
  })
  image[coordinates[0]][coordinates[1]] = 'X'
})


console.log(data.length + ' data points')
console.log(minmax)
console.log(image.map(function(row){return row.join('')}).join('\n'))

