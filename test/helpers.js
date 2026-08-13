// An RGBA image whose buffer really is width * height * 4 bytes.
exports.makeImage = function makeImage(width, height, fill = 0xff) {
  return { width, height, data: Buffer.alloc(width * height * 4, fill) }
}
