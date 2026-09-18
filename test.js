const test = require('brittle')
const jpeg = require('bare-jpeg')
const { resize } = require('.')

const grapefruit = require('./test/fixtures/grapefruit.jpg', {
  with: { type: 'binary' }
})

// An RGBA image whose buffer really is width * height * 4 bytes.
function makeImage(width, height, fill = 0xff) {
  return { width, height, data: Buffer.alloc(width * height * 4, fill) }
}

test('resize .jpg', (t) => {
  const resized = resize(jpeg.decode(grapefruit), 100)

  t.comment(resized)
})

test('resize to an explicit width keeps the aspect ratio', (t) => {
  const resized = resize(makeImage(8, 4), 4)

  t.is(resized.width, 4)
  t.is(resized.height, 2)
  t.is(resized.data.byteLength, 4 * 2 * 4)
})

test('resize to an explicit height keeps the aspect ratio', (t) => {
  const resized = resize(makeImage(8, 4), 0, 2)

  t.is(resized.width, 4)
  t.is(resized.height, 2)
})

test('resize by scale', (t) => {
  const resized = resize(makeImage(8, 4), { scale: 0.5 })

  t.is(resized.width, 4)
  t.is(resized.height, 2)
})

test('resize with an options object', (t) => {
  const resized = resize(makeImage(8, 4), { width: 2, height: 2 })

  t.is(resized.width, 2)
  t.is(resized.height, 2)
})

test('rejects a source buffer smaller than its dimensions', (t) => {
  t.exception(
    () => resize({ width: 64, height: 64, data: Buffer.alloc(4) }, 8),
    /Source buffer too small/
  )
})

test('rejects non-positive source dimensions', (t) => {
  for (const image of [
    { width: 0, height: 4 },
    { width: 4, height: 0 },
    { width: -4, height: 4 }
  ]) {
    t.exception(
      () => resize({ ...image, data: Buffer.alloc(64) }, { width: 2, height: 2 }),
      /Invalid source dimensions/
    )
  }
})

test('rejects a target that rounds down to zero', (t) => {
  t.exception(() => resize(makeImage(4, 4), { scale: 0.01 }), /Invalid target dimensions/)
})

test('rejects negative target dimensions', (t) => {
  t.exception(() => resize(makeImage(4, 4), -5), /Invalid target dimensions/)
})

test('rejects dimensions beyond the pixel cap', (t) => {
  t.exception(
    () => resize(makeImage(4, 4), { width: 100000, height: 100000 }),
    /dimensions exceed maximum/
  )
})
