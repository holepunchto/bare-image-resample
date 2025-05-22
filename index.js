const binding = require('./binding')

exports.resize = function resize(image, width = 0, height = 0, opts = {}) {
  if (typeof width === 'object') {
    opts = width
    width = opts.width || 0
    height = opts.height || 0
  }

  let scale = opts.scale || 1

  if (width) scale = width / image.width
  else if (height) scale = height / image.height

  if (width <= 0) width = image.width * scale
  if (height <= 0) height = image.height * scale

  width = Math.round(width)
  height = Math.round(height)

  const buffer = binding.resize(
    image.data,
    image.width,
    image.height,
    width,
    height
  )

  return {
    data: Buffer.from(buffer),
    width,
    height
  }
}
